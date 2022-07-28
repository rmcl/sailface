#include "debug.h"

void logDebugMessage(char *message) {
    //commsManager->sendDebugMessage(message);
}

void logDebugMessage(char c) {
    char msg[10];
    msg[0] = c;
    msg[1] = '\0';
    //commsManager->sendDebugMessage(msg);
}
void logDebugMessage(int number) {
    char numberMessage[16];
    itoa(number, numberMessage, 10);
    //commsManager->sendDebugMessage(numberMessage);
}
void logDebugMessage(uint32_t number) {
    char numberMessage[16];
    itoa(number, numberMessage, 10);
    //commsManager->sendDebugMessage(numberMessage);
}

void logDebugMessage(long number) {
    char numberMessage[16];
    ltoa(number, numberMessage, 10);
    //commsManager->sendDebugMessage(numberMessage);
}

/* dtoa implementation from this thread:
    https://forum.arduino.cc/t/convert-double-to-char/525251/5
*/
char* dtoa(double dN, char *cMJA, int iP) {
  char *ret = cMJA; long lP=1; byte bW=iP;
  while (bW>0) { lP=lP*10;  bW--;  }
  long lL = long(dN); double dD=(dN-double(lL))* double(lP);
  if (dN>=0) { dD=(dD + 0.5);  } else { dD=(dD-0.5); }
  long lR=abs(long(dD));  lL=abs(lL);
  if (lR==lP) { lL=lL+1;  lR=0;  }
  if ((dN<0) & ((lR+lL)>0)) { *cMJA++ = '-';  }
  ltoa(lL, cMJA, 10);
  if (iP>0) { while (*cMJA != '\0') { cMJA++; } *cMJA++ = '.'; lP=10;
  while (iP>1) { if (lR< lP) { *cMJA='0'; cMJA++; } lP=lP*10;  iP--; }
  ltoa(lR, cMJA, 10); }  return ret;
}

void logDebugMessage(double number) {
    char numberMessage[16];
    dtoa(number, numberMessage, 16);
    //commsManager->sendDebugMessage(numberMessage);
}


void logDebugMessage(float number, int num_digits_after_decimal) {
    char numberMessage[50];
    dtostrf(number,5, num_digits_after_decimal, numberMessage);
    //commsManager->sendDebugMessage(numberMessage);
}

void logDebugMessage(float number) {
    char numberMessage[50];
    dtostrf(number,5,2,numberMessage);
    //commsManager->sendDebugMessage(numberMessage);
}
