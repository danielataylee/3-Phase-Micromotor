//Master - Laptop: Comm12 / PC: Comm8 - Channel A (Blue)

/*
Date: May 2018
Organization: BCCRC Imaging Unit
Authors: Varalee Chinbomsom and Daniela Tay Lee Sanchez
Description: This script is for the master of the I2C connection
It should read input from the laster trigger output 
Calculate the number of samples in the sine lookup table from desired frequency
and pass that to the 3 slaves, so they can each generate a sine wave
to drive the dual beam micromotor
*/

#include <Wire.h> // Include the required Wire library for I2C connection


//Constants
#define DAC_RESOLUTION 12

//Variables
int i = 0; //Iterate through wave values for sine 
int sig = 1; //Like boolean 1, is true to start the slaves


//Metohd to configure Arduino
void setup() 
{
  Wire.begin(); //Start the I2C Bus as Master
  pinMode(DAC0, OUTPUT); //Set DAC0 as output
  pinMode(23, OUTPUT); //Set pin 23 as output

  analogWriteResolution(DAC_RESOLUTION); //Set up resolution

  //Send values to slave -> Determine phase offset from this delay and consider 
  Wire.beginTransmission(0x04); //transmit to device at address 0x04
  Wire.write(sig); //Send signal to start (1)
  Wire.endTransmission();    //Sop transmitting to slave1

  Wire.beginTransmission(0x06); //transmit to device at address 0x06
  Wire.write(sig); //Send signal to start (1)
  Wire.endTransmission();    //Sop transmitting to slave2  
}

//Method to loop indefinetely
void loop() 
{
    digitalWrite(23, LOW);
    delay(8000);
    digitalWrite(23, HIGH);
    delay(1); //1 millisec so it has time
    //digitalWrite(23, HIGH);
    
  //Should read laser trigger output to get frequency and then call method to determine number of samples and pass that to the slaves
}

//Method to determine number of samples from specified frequency
