#include <hidef.h>
#include "derivative.h"

//honestly if we're not exactly running low on ram, i'd rather
//just not have to deal with the buffer ever having the possiblity of 
//overflowing
#define SERIAL_BUFFER_SIZE 511

void updateDateString();
void initUART2(void);
void updateTimeString();
char* getCurrentTime(); 

//who needs ram? we need strings!
char serialBuffer[SERIAL_BUFFER_SIZE];
char dateString[7];
char timeString[7];
char currentTime[9];
long counter = 0;
int bufferPosition = 0;
int commaCount = 0; 


/* ISR triggered when the input register of SCI1 is full. All we do is take the value
  from the input buffer and put it into the serialBuffer variable.     
  
  Using the ISR in this way allows for the code to operate semi-autonomously and automatically parse
  the incomming GPS data. 
  
  The GPS module itself communicates over UART using a format called a "Nmea" string. We recieve 4 lines of data from 
  the gps module itself, but only care about one of them. we store each line in the serialBuffer variable since we have to recieve each byte one at a time
  
  An example NMEA String: 
    $GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76
    $GPGSA,A,3,10,07,05,02,29,04,08,13,,,,,1.72,1.03,1.38*0A
    $GPRMC,092750.000,A,5321.6802,N,00630.3372,W,0.02,31.66,280511,,,A*43
    $GPVTG,231.59,T,,M,0.30,N,0.56,K,A*31
    
    The relevant date and time information we want is stored in the GPRMC string, we take from this the values 092750.000 (time) and 280511 (date), 
    We use the commas as position indicators when parsing the string, ie. if we find that we're on the 10th comma we know that the most significant digit
    of the hour will be at commaPosition - 6 (string parsing uses this to good affect) 
    
    even though this ISR is more loaded than would be best practice it drives almost all the logic in this program therefore it's an acceptable compromise. 
    using the ISR in this manner also allows for other functionality of the clock to operate more-or-less at the same time without excessive polling requirements
    
  */
interrupt ((0x10000 - Vsci1) / 2 - 1) void SCI1_ISR(void) {
   
  //so for whatever reason you have to read SCI1SR1 for the flag to clear properly, i don't know the details really but datasheets says you need this
  int a = SCI1SR1;  
  
  //store the recieved byte in the serial buffer
  serialBuffer[bufferPosition] = SCI1DRL; 
  
  //This is where the data parsing happens, we count the commas in the nmea string
  //the number of commas are fixed, therefore we count them, if we have 2 commas we know
  //we're at the correct position for the time string, if we have 10 commas we're at the right position for the 
  //date string. We then use the current buffer position as a reference to parse out the desired data 
  //see the updateDateString() and updateTimeString() for data parsing
  if(serialBuffer[bufferPosition] == 44){
  //we detected a comma add to the count
    commaCount++; 

    //2 commas = possibly the time
    if(commaCount == 2){
      updateTimeString(); 
    }  
    //10 commas = possibly the date
    else if(commaCount == 10){
      updateDateString(); 
    }
    

  }
  
  //if a new line is detected reset the position in the data buffer, we only work with one line at a time
  //also reset the comma count
  if (serialBuffer[bufferPosition] == 13) {
    bufferPosition = 0;
    commaCount = 0; 
  } else {
    //prevent buffer from overflowing
    if (bufferPosition < SERIAL_BUFFER_SIZE - 1) {
      bufferPosition++;
    }
  } 

}

//formats the GPS time given in UTC to EST, updates currentTime variable
//offset needs to be changed inside function if required. 
void setCurrentTime(){

  //convert char hour value to integer so we can perform math on it easier
  int currentHour = (timeString[0] - 48) * 10 + (timeString[1] - 48);
  int a = 0; 
  
  //get currentTime string, we will use this as a starting point  
  for(a = 0; a < 7; a++){
    currentTime[a] = timeString[a]; 
  }
  
  //number of hours to set back from UTC, currently that is 4 with daylight savings
  currentHour = currentHour - 4; 
  
  //subtraction may give us a negative value, if this happens then rollover the value
  if(currentHour < 0){
    currentHour = 24 + currentHour;   
  }
  
  //make it a 12 hour clock since that's what everyone's use to 
  //Also add PM and AM to the current string depending on the time
  if(currentHour > 12){
    currentHour -= 12; 
    currentTime[6] = 'P'; 
    currentTime[7] = 'M'; 
  }else{
    currentTime[6] = 'A'; 
    currentTime[7] = 'M'; 
  }
  
  //convert the integer hour value back into a character and 
  //place it back in the currentHour String
  currentTime[0] = (currentHour/10) + 48;
  currentTime[1] = (currentHour%10) + 48;
}

//returns the indicated character in the currentTime String
//easier to set this up as an interface than it is to create anything fancy
//note format is HHMMSSxM where x is either A or P
char getCurrentTimeCharacter(int index){
 return currentTime[index]; 
}


//inits UART on the second UART port, configures the uart output to a baud rate of 9600 and enables the interrupt
//to allow for us to buffer incomming data.
void initUART2(void) {
  SCI1BDH = 0x00;
  SCI1BDL = 156;

  SCI1CR1 = 0x00; //8 bit data frame, no parity bit, no loop;

  SCI1CR2 |= (1 << 2); //Recieve Enable
  SCI1CR2 |= (1 << 5); //Reciever Full Interrupt Enable
  __asm CLI; //enable global interrupts
}

/*Code warrior doesn't really allow us to have global variables therefore to access the serialBuffer we need a function that can
retrieve the values from the buffer*/
char getBufferCharacter(int index) {
  return serialBuffer[index];
}


/*  Since we can't access the string directly outside of this file this function is designed to allow
for the rest of the program to access the timeString and read its values. */
char getTimeStringCharacter(int index){
 return timeString[index]; 
}

/*returns the given index of a character in the date string
  Date string is in format DDMMYY
*/
char getDateStringCharacter(int index){
 return dateString[index]; 
}
                 
     
//tests whether a given character is a number or not
int isANumber(char c){
    return (c >= '0' && c <= '9'); 
}
                
/* The GPS module being used to read the time outputs "nmea Strings" since we only want to parse out a few bits of data
namely the time we use this function. it is called sparingly in the ISR depending on the number of commas present 
Both the time and date information are stored in the $GPRMC nmea string so we parse from tha
*/
void updateTimeString(){
  //we want to make sure we're checking the correct, if the serial buffer is currently being filled with the GPRMC nmea string then 
  //the serial buffer will start with this string. we can check for that.  
  char startString[7] = "$GPRMC";
  int a = 0; 
  int correct = 1;
  
  //check every bit against the start string
  for( a = 0; a < 6; a++){
    //there's a small issue with the serial buffer, the first index will not fill with the correct value. We've been 
    //unable to track it down hence why we're using a+1 here in the serial buffer
    //check the beginning of the serial buffer against the start string
    if(!(serialBuffer[a + 1] == startString[a])){
      //if even one character doesn't match just quit
      correct = 0;
      return;
    }
  }
 
  //the time will be given in the format HHMMSS.000, we know the current position is relative to one of the commas in the 
  //NMEA string, so we just parse out the specific values we want as they relate. the resulting timeString is in format
  //HHMMSS, by default the values are given in UTC 
  if( isANumber(serialBuffer[bufferPosition - 10]) &&
  isANumber(serialBuffer[bufferPosition - 9]) &&
  isANumber(serialBuffer[bufferPosition - 8]) &&
  isANumber(serialBuffer[bufferPosition - 7]) &&
  isANumber(serialBuffer[bufferPosition - 6]) &&
  isANumber(serialBuffer[bufferPosition - 5])){
    
     timeString[0] = serialBuffer[bufferPosition - 10];
     timeString[1] = serialBuffer[bufferPosition - 9];
     timeString[2] = serialBuffer[bufferPosition - 8];
     timeString[3] = serialBuffer[bufferPosition - 7];
     timeString[4] = serialBuffer[bufferPosition - 6];
     timeString[5] = serialBuffer[bufferPosition - 5];
     
     setCurrentTime(); //update the currentTime string so that we can get the desired timezone
  }
}



/* This function is called from the UART ISR and parses the date string out of the NMEA string returned. 
For more information please see the comments on the ISR and the updateTimeString, this function operates almost identically
*/
void updateDateString(){
  //serial buffer must start with $GPRMC
  char startString[7] = "$GPRMC";
  int a = 0; 
  int correct = 1;
  for( a = 0; a < 6; a++){
  //check values against the start string, exit if they don't match
    if(!(serialBuffer[a + 1] == startString[a])){
      correct = 0;
      return;
    }
  }
  
  //parse data out of the date string by default these values are given as MMDDYY
     dateString[0] = serialBuffer[bufferPosition - 6];
     dateString[1] = serialBuffer[bufferPosition - 5];
     dateString[2] = serialBuffer[bufferPosition - 4];
     dateString[3] = serialBuffer[bufferPosition - 3];
     dateString[4] = serialBuffer[bufferPosition - 2];
     dateString[5] = serialBuffer[bufferPosition - 1];
}
