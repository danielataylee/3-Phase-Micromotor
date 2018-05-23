//Master - Laptop: Comm12 / PC: Comm8 - Channel A (Blue)

/*
Date: May 2018
Organization: BCCRC Imaging Unit
Authors: Varalee Chinbomsom and Daniela Tay Lee Sanchez
Description: This script is for the master of the I2C connection
It should get the number of samples and send it to the slaves
*/

#include <Wire.h> // Include the required Wire library for I2C connection

//Constants
#define DAC_RESOLUTION 12 //12 bits -> max num is 4095
#define PIN_SIGNAL 49 //Pin to signal when to write output
#define S1_ADDRESS 0x04
#define S2_ADDRESS 0x06
#define S3_ADDRESS 0x

//Signal for the slaves so know which table to pick
unsigned char sig2slaves = 0; //Has to be unsigned char bc I2C transfers 1 byte (8 bits), int data type takes 2 bytes
/*
 Sig -> table
  1  -> 4000
  2  -> 2000
  3  -> 1000
  4  -> 800
  5  -> 625
  6  -> 500
  7  -> 400
  8  -> 333
  9  -> 286
  10  -> 250
*/

int sent = 1; //So it knows when to transfer signal, and only once
int k=0; //to test

//Metohd to configure Arduino
void setup() 
{
  Wire.begin(); //Start the I2C Bus as Master
  //Set pins as output
  pinMode(PIN_SIGNAL, OUTPUT); 
  //Set up resolution
  analogWriteResolution(DAC_RESOLUTION); 
}


//Method to loop indefinetely
void loop() 
{
    digitalWrite(PIN_SIGNAL, LOW); 
    digitalWrite(PIN_SIGNAL, HIGH);
    
    //JUST TESTING -> after certain time, change number of samples
    //ACTUALLY SHOULD DO: If lase trigger output changes
    if (k == 512000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 1;
    }
    else if (k == 1536000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 2;
    }
    else if (k == 2048000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 3;
    }
    else if (k == 2560000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 4;
      
    }
    else if (k == 3000000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 5;
    }
    else if (k == 3500000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 6;
    }
    else if (k == 4000000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 7;
    }
    else if (k == 4500000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 8;
    }
    else if (k == 5000000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 9;
    }
    else if (k == 5500000)
    {
      sent = 0; //So knows to send new value
      sig2slaves = 10;
    }

    //If need to send to slaves
    if (sent == 0) 
    {
      //Send value to slave1
      Wire.beginTransmission(S1_ADDRESS); 
      Wire.write(sig2slaves); 
      Wire.endTransmission();   
      //Send value to slave1
      Wire.beginTransmission(S2_ADDRESS); 
      Wire.write(sig2slaves); 
      Wire.endTransmission();   
      //Send value to slave3
      //Wire.beginTransmission(S3_ADDRESS); 
      //Wire.write(sig2slaves); 
      //Wire.endTransmission(); 
      sent=1; //Done sending
    }
   
    k++; //just for testing
}
