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
int k=0;

//Metohd to configure Arduino
void setup() 
{
  Wire.begin(); //Start the I2C Bus as Master
  pinMode(23, OUTPUT); //Set pin 23 as output
  pinMode(49, OUTPUT); //Set pin 49 as output
  pinMode(43, OUTPUT); //Set pin 49 as output

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
    //LOW TO HIGH -> Make the DAC output from sine lookup table
    

    //JUST TESTING -> ACTUALLY SHOULD DO: If lase trigger output changes
    if (k<512000)//Just testing that changes frequency
    { //512
    digitalWrite(43, HIGH); //Tell to change samples num
    digitalWrite(23, LOW);
    digitalWrite(23, HIGH);
    }

    else //1024
    {
      digitalWrite(43, LOW);
      digitalWrite(49, LOW); 
      digitalWrite(49, HIGH);
    }
    k++;
}
