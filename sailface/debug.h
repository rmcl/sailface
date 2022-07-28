#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

void logDebugMessage(char c);
void logDebugMessage(int number);
void logDebugMessage(uint32_t number);
void logDebugMessage(float number);
void logDebugMessage(float number, int num_digits_after_decimal);
void logDebugMessage(long number);
void logDebugMessage(double number);
void logDebugMessage(char *message);
