//Arduino2

/*
Date: May 2018
Organization: BCCRC Imaging Unit
Authors: Varalee Chinbomsom and Daniela Tay Lee Sanchez
Purpose:     To drive the dual beam micromotor catheter (Namiki Precision) that needs 3 sine waves (Use 1 Arduino for each)
Description: The script generates a sine lookup table of number of samples SAMPLES and offset degrees OFFDEG
              The maximum number is 4095 because it uses a 12 bit DAC to output the values. Therefore, the amplitude is approx 2.7 V.
              It uses PIN_SIGNAL to read the trigger input, so it has an interrupt for when it goes from LOW to HIGH
              When mastersig is 1 it starts writing the values to the DAC, skipping clock pulses so it ramps up to the 
              final frequency by 10 Htz each time. 
              The RESET_IN has another interrupt to reset the variables and restart the sine wave.
*/

//Constants
#define SAMPLES 1000
#define OFFDEG 120

#define DAC_RESOLUTION 12 //12 bits
#define PIN_SIGNAL 51 //read trigger input
#define RESET_OUT 31 //This pin outputs high and is used for the switch, only one arduino needs it but the code should be the same so the frequencies are the same among all 3 arduinos 
#define RESET_IN 23
#define DAC DAC1


//Variables -> Some declared as volatile because used in interrput
volatile int i = 0; //Iterate through wave values for sine wave
volatile unsigned char mastersig = 0; //Signal to receive to start outputting
volatile int steps = 10000/SAMPLES; //Steps to ramp up
volatile int k = 0; //Used for syncing the start
volatile int count = 0; 
volatile int pulse = 0; 
volatile int out = 0; //Start skipping this many ticks
volatile int points = 0; //Track points written to DAC
volatile int cycles = 100; //Num of cycles to complete on each ramp up
int j = 0; //Used when generating sine lookup table
int offset = OFFDEG*SAMPLES/360; //Calculate offset position -> Formula: offset degrees * num of samples / 360
int bitMul = (1<<12)/2; //1<<12 Is equivalent to 2^12 = 4096

//Lookup table
static int wave[SAMPLES]; 


//Method to configure Arduino
void setup() 
{
   //Set pins as inputs or outputs
   pinMode(RESET_OUT, OUTPUT);
   pinMode(RESET_IN, INPUT);
   pinMode(DAC, OUTPUT);   
   pinMode(PIN_SIGNAL, INPUT); 
   //Set up resolution
   analogWriteResolution(DAC_RESOLUTION); 
   //Call method to generate sine lookup table
   generateSineTable(offset, wave, SAMPLES); 
   //Set pin as high so used with switch
   digitalWrite(RESET_OUT, HIGH); 
   //Initialize interrupt service routines
   attachInterrupt(digitalPinToInterrupt(PIN_SIGNAL), TRIGGER_ISR, RISING); //Call TRIGGER_ISR when PIN_SIGNAL goes from low to high
   attachInterrupt(digitalPinToInterrupt(RESET_IN), RESET_ISR, CHANGE); //Call RESET_ISR when RESET_IN changes
}

//Method to generate sine lookup table
void generateSineTable(int off, int * table, int s)
{
  for (i = off ; i < (s + off) ; i++)
    table[j++] = (round(((sin(2*PI*i/s))*(bitMul-1)))+bitMul);
  //Reset variables
  i=0;
  j=0;
}


//Method that loops indefinetely
void loop() 
{
    //Start after that many clock ticks
    if (k == 50000) 
      mastersig = 1; //Signal to start outputting
}


//Interrupt method, everytime the pin goes low to high
void TRIGGER_ISR() 
{
  k++;
  
  if (mastersig == 1) 
  {
    //If need to output a point
    if ((count%steps == 0) || (pulse <= out)) //Ex: 1000 samples -> steps = 10, Output every multiple of 10 point, or if increasing frequency -> output until pulse = out
    {
      analogWrite(DAC, wave[i]); //Write on DAC
      i = (i+1)%SAMPLES; //next value in lookup table
      points++; //Wrote a point
      if (count%steps == 0)
        pulse = 0;
      pulse++; //Point written
    }

    //If done number of cycles, restart with next ramp up
    if (points == (SAMPLES*cycles) && (out < steps))
    {
      points = 0;
      count = -1; 
      out++; 
    }
    
    count++;
    
  }//End if mastersig is 1

  //Just output value 0 (line) if signal to start is not
  else
    analogWrite(DAC, 0);
}


//Interrupt method to reset variables
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
