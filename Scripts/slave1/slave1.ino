//Slave1 - Comm15 - Channel B (Red)

/*
Date: May 2018
Organization: BCCRC Imaging Unit
Authors: Varalee Chinbomsom and Daniela Tay Lee Sanchez
Description: This script is for slave1 of the I2C connection
To receive a signal from the master and start generating the sine wave 
The signal should be the number of samples in the sine lookup table
*/

# include <Wire.h> // Include the required Wire library for I2C

//Constants
#define DAC_RESOLUTION 12
#define I2C_ADDRESS 0x04 //Can use script I2C_DEVICE_SCANNER to get address
#define AMPLITUDE 7 //Gets it to 2.5V with 2^8 -> need 3V, but AnalogWrite only 12 bits so can only get to 4096
#define PI 3.14159265358979323846

//Variables
int maxSamples = 64; //# elements in sine lookup table
int i = 0; //Iterate through wave values for sine wave
int offsetdeg = 0; //120 degrees 
int offset;
int x = 0; //Signal to receive from master
int bitMul = 1<<8; //Is equivalent to 2^8 = 256
int sineval;

//Method to configure Arduino
void setup() 
{
   //Start the I2C Bus as Slave on address 
   Wire.begin(I2C_ADDRESS); 

   pinMode(DAC0, OUTPUT);  
   analogWriteResolution(DAC_RESOLUTION); //Set up resolution
   offset = offsetdeg*maxSamples/360; //Calculate offset
   i = offset; //start at offset
}

//Method to set the signal to what was transfered by master
void receiveEvent(int bytes) 
{
  x = Wire.read(); //Read one character from the I2C master and assign to x
}

//Method to loop indefinetely
void loop() 
{
    Wire.onReceive(receiveEvent); //Receive signal from master
      
    if (x == 1) //If the signal was true, start generating wave
    {
      sineval = (AMPLITUDE*(round(((sin(2*PI*i/maxSamples))*bitMul))+ bitMul));
      analogWrite(DAC0, sineval); //Write every element of sinewave
      i = (i+1)%(maxSamples+offset); // Next element, and if reached last (considering offset), go back to 0
    }    
}
