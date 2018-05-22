//Slave1 - Laptop: Comm15 / PC: Comm4 - Channel A (Blue)

/*
Date: May 2018
Organization: BCCRC Imaging Unit
Authors: Varalee Chinbomsom and Daniela Tay Lee Sanchez
Description: This script is for slave1 of the I2C connection
To receive a signal from the master and start generating the sine wave 
The signal should be the number of samples in the sine lookup table
*/

#include <Wire.h> // Include the required Wire library for I2C

//Constants
#define DAC_RESOLUTION 12
#define I2C_ADDRESS 0x04 //Can use script I2C_DEVICE_SCANNER to get address
#define AMPLITUDE 7 //Gets it to 2.5V with 2^8 -> need 3V, but AnalogWrite only 12 bits so can only get to 4096
#define PI 3.14159265358979323846
#define MAX_SAMPLES 8192 //Wont go further than this

//Variables
int maxSamples = 1024; //# elements in sine lookup table
int i = 0; //Iterate through wave values for sine wave
int j=0; //for generating values with offset
int offsetdeg = 0; //degrees 
int offset; //Formula: offset degrees * num of samples / 360
int x = 0; //Signal to receive from master
int bitMul = 1<<8; //Is equivalent to 2^8 = 256
int pinSig = 0;
int pinSig2 = 0;

volatile int pinState = 0;

/*
int st = 0;
int et = 0;
int z=0;
int p=0;
*/
static int sinewave[MAX_SAMPLES];

//Method to configure Arduino
void setup() 
{
   Serial.begin(9600);
   //Start the I2C Bus as Slave on address 
   Wire.begin(I2C_ADDRESS); 

   pinMode(DAC1, OUTPUT);  
   pinMode(39, INPUT);  
   pinMode(51, INPUT); 
    
   analogWriteResolution(DAC_RESOLUTION); //Set up resolution
   offset = offsetdeg*maxSamples/360; //Calculate offset

  attachInterrupt(0, pin_ISR, FALLING);

}

//Method to set the signal to what was transfered by master
void receiveEvent(int bytes) 
{
  x = Wire.read(); //Read one character from the I2C master and assign to x
}


//Method to loop indefinetely
void loop() 
{
//    Wire.onReceive(receiveEvent); //Receive signal from master
//    pinSig = digitalRead(39);
//    pinSig2 = digitalRead(51);
//
//    if (x > 0) //If the signal was true, start generating wave
//    {
//      //Serial.print(x);
//      //Serial.print("    ");
//      if(pinSig == 1)
//        i=0;
//      if(pinSig2 == 1)
//        changeFreq(512); //Pass number of samples you want 
//        
//      analogWrite(DAC1, sinewave[i]); //Write every element of sinewave 
//      i = (i+1)%maxSamples; // Next element, and if reached last, go back to 0
//    }
}
void pin_ISR() {
  pinState = digitalRead(51);
  i = i+1
  analogWrite(DAC1, sinewave[i])
  
}
