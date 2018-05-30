//Slave1 - PC: Comm8 - Channel A (Blue)

/*
Date: May 2018
Organization: BCCRC Imaging Unit
Authors: Varalee Chinbomsom and Daniela Tay Lee Sanchez
Description: The script generates a sine wave with amplitude between 2.5 and 3 V, 
            of # samples SAMPLES and offset degrees offsetdeg
*/

//Constants
#define SAMPLES 1000
#define OFFDEG 0

#define DAC_RESOLUTION 12
#define PIN_SIGNAL 51
#define RESET_OUT 31
#define RESET_IN 23
#define DAC DAC1


//Variables
volatile int i = 0; //Iterate through wave values for sine wave
volatile unsigned char mastersig = 0; //Signal to receive from master
volatile int steps = 10000/SAMPLES;
volatile int k = 0; //For testing fixed
volatile int count = 0; //counts clock ticks to skip
volatile int pulse = 0; //Start skipping this many ticks
volatile int out = 0; //Start skipping this many ticks
volatile int points = 0; //Points outputted
volatile int cycles = 100; //Num of cycles to complete on each ramp up
int j = 0;
int offset = OFFDEG*SAMPLES/360; //Calculate offset -> Formula: offset degrees * num of samples / 360
int bitMul = (1<<12)/2; //Is equivalent to 2^12=4096

//Lookup tables
static int wave[SAMPLES]; //To choose table

//Method to configure Arduino
void setup() 
{
   //Set pins as inputs or outputs
   pinMode(RESET_OUT, OUTPUT);
   pinMode(RESET_IN, INPUT);
   pinMode(DAC, OUTPUT);   
   pinMode(PIN_SIGNAL, INPUT); 
   analogWriteResolution(DAC_RESOLUTION); //Set up resolution

   generateSineTable(offset, wave, SAMPLES); //Call method to generate sine lookup
    
   digitalWrite(RESET_OUT, HIGH);
   attachInterrupt(digitalPinToInterrupt(PIN_SIGNAL), TRIGGER_ISR, RISING); //Call pin_ISR when 51 goes from low to high
   attachInterrupt(digitalPinToInterrupt(RESET_IN), RESET_ISR, CHANGE); //Call pin_ISR when 51 goes from low to high
}


void generateSineTable(int off, int * table, int s)
{
  for (i=off ; i < s + off ; i++)
  {
    table[j++] = (round(((sin(2*PI*i/s))*(bitMul-1)))+bitMul);
  }
  i=0;
  j=0;
}


//Method to loop indefinetely
void loop() 
{
    //Start after 1000 clock ticks
    if (k == 50000) 
      mastersig = 1; //start
}

//Interrupt method, everytime the pin goes low to high
void TRIGGER_ISR() 
{ 
  
  k++;
  
  if (mastersig == 1) 
  {
    if ((count%steps == 0) || (pulse <= out))
    {
      analogWrite(DAC, wave[i]); //Write on DAC
      i = (i+1)%SAMPLES;
      points++; //Wrote a point
      if (count%steps == 0)
        pulse = 0;
      pulse++;
    }

    //If done num of cycles, restart with next pulse
    if (points == (SAMPLES*cycles) && (out < steps))
    {
      points = 0;
      count = -1; 
      out++; //Up until pulse=1
    }
    
    count++;
    
  }//End if mastersig is 1

  //Just output value 0 (line) if signal to start is not
  else
    analogWrite(DAC1, 0);
}

//When pin changes reset
void RESET_ISR() 
{
  mastersig=0;
  i=0;
  count=0;
  pulse=0;
  out=0;
  points=0;
  k=0;
}

