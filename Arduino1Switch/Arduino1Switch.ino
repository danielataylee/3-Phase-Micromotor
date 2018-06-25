//Arduino1

/*
Date: June 2018
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
#define MAXSAMPLES 6000
#define OFFDEG 0

#define DAC_RESOLUTION 12 //12 bits
#define PIN_SIGNAL 51 //read trigger input
#define RESET_OUT 31 //This pin outputs high and is used for the switch, only one arduino needs it but the code should be the same so the frequencies are the same among all 3 arduinos 
#define RESET_IN 23
#define DAC DAC0
#define CHG_SAMPLES 35 //Pin high when want to change num of samples


//Variables -> Some declared as volatile because used in interrput
volatile int samples = MAXSAMPLES;
volatile int i = 0; //Iterate through wave values for sine wave
volatile int steps = 10000/MAXSAMPLES; //Steps to ramp up
volatile int count = 0; 
volatile int pulse = 0; 
volatile int out = 0; //Start skipping this many ticks
volatile int points = 0; //Track points written to DAC
volatile int cycles = 100; //Num of cycles to complete on each ramp up
int j = 0; //Used when generating sine lookup table
int offset = OFFDEG*MAXSAMPLES/360; //Calculate offset position -> Formula: offset degrees * num of samples / 360
int bitMul = (1<<12)/2; //1<<12 Is equivalent to 2^12 = 4096

//Lookup table
static int wave[MAXSAMPLES]; 


//Method to configure Arduino
void setup() 
{
   //Set pins as inputs or outputs
   pinMode(RESET_OUT, OUTPUT);
   pinMode(RESET_IN, INPUT);
   pinMode(DAC, OUTPUT);   
   pinMode(PIN_SIGNAL, INPUT); 
   pinMode(CHG_SAMPLES, INPUT);
   //Set up resolution
   analogWriteResolution(DAC_RESOLUTION); 
   
   //Initiliaze USB
   SerialUSB.begin(2000000); //Native USB port
   while(!SerialUSB); //While connection is being established
   //Set num of samples
   setSamples(SerialUSB.read());

   //Set pin as high so used with switch
   digitalWrite(RESET_OUT, HIGH); 
   //Initialize interrupt service routines
   attachInterrupt(digitalPinToInterrupt(PIN_SIGNAL), TRIGGER_ISR, RISING); //Call TRIGGER_ISR when PIN_SIGNAL goes from low to high
   attachInterrupt(digitalPinToInterrupt(RESET_IN), RESET_ISR, LOW); //Call RESET_ISR when RESET_IN is low (off)
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

//Method to change num of samples
void setSamples(int s)
{
  samples = s;//Set new num of samples
  //calculate new params with new num of samples
  steps = 10000/samples;
  offset = OFFDEG*samples/360;
  //Call method to generate sine lookup table
  generateSineTable(offset, wave, samples); 
}

//Method that loops indefinetely
void loop() 
{
}


//Interrupt method, everytime the pin goes low to high
void TRIGGER_ISR() 
{  
  if ((digitalRead((RESET_IN))) == HIGH) //If switch is on, produce output
  {
    //If need to output a point
    if ((count%steps == 0) || (pulse <= out)) //Ex: 1000 samples -> steps = 10, Output every multiple of 10 point, or if increasing frequency -> output until pulse = out
    {
      analogWrite(DAC, wave[i]); //Write on DAC
      i = (i+1)%samples; //next value in lookup table
      points++; //Wrote a point
      if (count%steps == 0)
        pulse = 0;
      pulse++; //Point written
    }

    //If done number of cycles, restart with next ramp up
    if (points == (samples*cycles) && (out < steps))
    {
      points = 0;
      count = -1; 
      out++; 
    }
    
    count++;
    
  }//End if reset in is 1

  //Just dont output anything otherwise
}


//Interrupt method to reset variables when switch is off
void RESET_ISR() 
{
  i=0;
  count=0;
  pulse=0;
  out=0;
  points=0;

  //If want to change num of sampels
  if ((digitalRead(CHG_SAMPLES) == HIGH))
  {
    //Read the specific num of samples from USB and set num of samples
    setSamples(SerialUSB.read());
  }
}
