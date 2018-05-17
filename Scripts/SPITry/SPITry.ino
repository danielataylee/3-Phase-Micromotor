#include <SPI.h>

//DAC SPECS: Must remain low until 16 bits are transferred
//Data clocked into register on rising edge

#define PIN_SS 10
#define LDAC 8

uint8_t bitWord11 = B00011000; //a
uint8_t bitWord12 = B10000000; 
uint8_t bitWord21 = B01010000; //b
uint8_t bitWord22 = B00000000; 
uint8_t bitWord31 = B10001000; //c
uint8_t bitWord32 = B00000001; 

uint8_t bitWord41 = B00100000; //c
uint8_t bitWord42 = B00000000; 
SPISettings settingsA(10000000, MSBFIRST, SPI_MODE0); //msbfirst: data shifted using most significant bit (left)
//CLOCK POLARITY: 0, CLOCK PHASE: 0, OUT EDGE: FALLING, DATA CAPTURE: RISING    

void setup() 
{
  pinMode(LDAC, OUTPUT);
  pinMode(PIN_SS, OUTPUT); 
  digitalWrite(LDAC, HIGH); //LDAC low -> 0 (Immediatly update) - 1 (DAC output unchaged)
  SPI.begin(PIN_SS);
  SPI.setClockDivider(48); //bc arduino is 16 MHz and MAX DAC is max 10MHz so now we get 8
  digitalWrite(PIN_SS, HIGH);
}

void loop() 
{

  SPI.beginTransaction(settingsA);
  digitalWrite(LDAC, LOW); //LDAC low -> 0 (Immediatly update) - 1 (DAC output unchaged)
  
  digitalWrite(PIN_SS, LOW);
  SPI.transfer(bitWord31); //send value -> SPI_CONTINUE keep chip select
  SPI.transfer(bitWord32); //send value
  digitalWrite(PIN_SS, HIGH);
  /*
  SPI.transfer(PIN_SS, bitWord21, SPI_CONTINUE); //send value -> SPI_CONTINUE keep chip select
  SPI.transfer(PIN_SS, bitWord22, SPI_LAST); //send value
  SPI.transfer(PIN_SS, bitWord31, SPI_CONTINUE); //send value -> SPI_CONTINUE keep chip select
  SPI.transfer(PIN_SS, bitWord32, SPI_LAST); //send value

  digitalWrite(PIN_SS, LOW);
  SPI.transfer(bitWord41); //send value -> SPI_CONTINUE keep chip select
  SPI.transfer(bitWord42); //send value
  digitalWrite(PIN_SS, HIGH); */
  SPI.endTransaction();
}
