//Master - Comm12 - Channel A (Blue)

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
#define MAX_SAMPLES 1024 //# of elements in sinewave lookup table -> determines the frequency
#define DAC_RESOLUTION 12

//Variables
int i = 0; //Iterate through wave values for sine 
int offsetdeg = 120; //120 degrees 
int offset; ////Formula: offset degrees * num of samples / 360
int start = 1; //Like boolean 1, is true to start the slaves


//Metohd to configure Arduino
void setup() 
{
  Wire.begin(); //Start the I2C Bus as Master
  pinMode(DAC0, OUTPUT); //Set DAC0 as output
  analogWriteResolution(DAC_RESOLUTION); //Set up resolution
  offset = offsetdeg*MAX_SAMPLES/360; //Calculate offset starting position

  //Send values to slave -> Determine phase offset from this delay and consider 
  Wire.beginTransmission(0x04); //transmit to device at address 0x04
  Wire.write(start); //Send signal to start (1)
  Wire.endTransmission();    //Sop transmitting to slave1

  Wire.beginTransmission(0x06); //transmit to device at address 0x06
  Wire.write(start); //Send signal to start (1)
  Wire.endTransmission();    //Sop transmitting to slave2  
}

//Method to loop indefinetely
void loop() 
{
  //i=(i+1)%MAX_SAMPLES;//Next element, and if reached last, go back to 0
  //analogWrite(DAC0, sinewave[i]); //Write every element of sinewave1

  //Should read laser trigger output to get frequency and then call method to determine number of samples and pass that to the slaves
}

//Method to determine number of samples from specified frequency
