ECE3720 Final Project Abstract – Space Time 
Matthew James Bellafaire
Nicholas Musienko

The goal of the project is to create a simple alarm clock utilizing an SPI GPS module to acquire the GPS time signal. To do this, the GPS data will be parsed to extract the time component, and that time will then be used to drive the clock. The entire solution will be powered from a single external power supply, hopefully via USB power, but a larger supply may be necessary. To display the time a 1.2” seven segment display will be interfaced externally via I2C to the HCS12. In addition to the seven-segment display another 8x8 LED matrix will be added (interfaced by I2C) to display AM/PM and whether an alarm is set. User input will be provided utilizing 4 external illuminated buttons. 
As a stretch goal the hardware, including the dragon-12, will be encased in a 3D printed enclosure.  Included below are links to external hardware we are planning to use in the final project, this list is not yet final.
GPS Module: https://www.adafruit.com/product/746
Possible Display: https://www.adafruit.com/product/1270
Possible Buttons: https://www.adafruit.com/product/3489
Connectors for Buttons: https://www.adafruit.com/product/3835
LED Matrix for AM/PM and Alarm set: https://www.adafruit.com/product/1049 

