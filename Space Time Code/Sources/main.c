#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "declarations.h"


void main(void) {



	EnableInterrupts;


  for(;;) {
    _FEED_COP();
  }
}
