#include <SPI.h>


void setup(){
  // initialize the bus for a device on pin 10
  SPI.begin(10);
}

void loop(){
//A single byte transfer to a device on pin 10 could look like this :
byte response = SPI.transfer(10, 0xFF);
  
//transfer 0x0F to the device on pin 10, keep the chip selected
//SPI.transfer(10, 0xF0, SPI_CONTINUE);
//transfer 0x00 to the device on pin 10, keep the chip selected
//SPI.transfer(10, 0×00, SPI_CONTINUE);

//transfer 0x00 to the device on pin 10, store byte received in response1, keep the chip selected
//byte response1 = SPI.transfer(10, 0×0F, SPI_CONTINUE);
//transfer 0x00 to the device on pin 10, store byte received in response2, deselect the chip
//byte response2 = SPI.transfer(10, 0×00);
}

