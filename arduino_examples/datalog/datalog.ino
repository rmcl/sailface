#include <Arduino.h>
#include <SPI.h>
#include "SdFat.h"


#define error(msg) (Serial.println(F(msg)),errorHalt())

// Digital pin to indicate an error, set to -1 if not used.
// The led blinks for fatal errors. The led goes on solid for SD write
// overrun errors and logging continues.
const int8_t ERROR_LED_PIN = -1;
const uint8_t SD_CS_PIN = 10;

// Maximum file size in bytes.
// The program creates a contiguous file with MAX_FILE_SIZE_MiB bytes.
// The file will be truncated if logging is stopped early.
const uint32_t MAX_FILE_SIZE_MiB = 100;  // 100 MiB file.
const uint32_t MAX_FILE_SIZE = MAX_FILE_SIZE_MiB << 20;

// Maximum length name including zero byte.
//const size_t FIFO_SIZE_BYTES = 12*512;

// Max SPI rate for AVR is 10 MHz for F_CPU 20 MHz, 8 MHz for F_CPU 16 MHz.
// this seems to suggest max is half CPU clock https://forum.arduino.cc/t/arduino-mega-2560-cant-reach-max-spi-sck-frequency/627085
#define SPI_CLOCK SD_SCK_MHZ(4)
//something to try below if no worky
//#define SPI_CLOCK SD_SCK_MHZ(4)
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)

SdExFat sd;
ExFile csvFile;


void errorHalt() {
  sd.printSdError(&Serial);
  csvFile.close();
}

bool openForAppend(ExFile *csvFp) {

  if (!csvFp->open("hello.txt", O_WRONLY|O_CREAT|O_APPEND)) {
      error("open csvFile failed");
  }

  return true;
}

void setup() {
    Serial.begin(9600);

    delay(1000);

    if (!sd.cardBegin(SD_CONFIG)) {
       Serial.println(
          String("\nSD initialization failed.\n") + \
          String("Do not reformat the card!\n") + \
          String("Is the card correctly inserted?\n") + \
          String("Is there a wiring/soldering problem?\n"));

       errorHalt();
       return;
   }

   //sd.chvol();
   delay(1000);

   uint32_t size = sd.card()->sectorCount();
   if (size == 0) {
       Serial.println("Can't determine the card size.");
       errorHalt();
       return;
   }
   uint32_t sizeMB = 0.000512 * size + 0.5;
   Serial.println(String("Card size: ") + String(sizeMB));
   Serial.println(String("Volume is FAT") + String(int(sd.fatType())));



   //sd.ls(&Serial, LS_R | LS_DATE | LS_SIZE);
   delay(5000);

   openForAppend(&csvFile);
   Serial.println("FILE OPENED");
   delay(5000);
}

void loop() {
    Serial.println("WRITING");
    csvFile.write("HELLO WORLD,PARTY\n");
    delay(5000);
}
