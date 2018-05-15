//This script generates a sine lookup table based on number of samples

//Constants
#define MAX_SAMPLES 4096 //There are 120 elements in array
#define DAC_RESOLUTION 12
#define AMPLITUDE 7 //Gets it to 2.5V with 2^8 -> need 3V, but AnalogWrite only 12 bits so can only get to 4096
#define PI 3.14159265358979323846

//Variables
int i = 0; //Iterate through wave values for wave 1
int offsetdeg = 120; //120 degrees 
int offset; //Formula: offset degrees * num of samples / 360
int bitMul = 1<<8; //Is equivalent to 2^8 = 256

//sine wave lookup table
static int sinewave[MAX_SAMPLES];

void setup() 
{
  //Set DACs as outputs
  pinMode(DAC0, OUTPUT);  
  analogWriteResolution(DAC_RESOLUTION); //Set up resolution
  offset = offsetdeg*MAX_SAMPLES/360; //Calculate offset

  //Call method to generate sine lookup table
  generateSineTable(offsetdeg, sinewave, MAX_SAMPLES, AMPLITUDE);
}

void loop() 
{
    i = (i+1)%MAX_SAMPLES; // Next element, and if reached last, go back to 0

    analogWrite(DAC0, sinewave[i]); //Write every element of sinewave
}


//Method to generate a sine table from the specified offset, and number of samples
//off is the offset, table is a pointer to the sine lookup table array and samples is the num of elements
void generateSineTable(int off, int * table, int samples, int a)
{
  for (i=0 ; i < samples ; i++)
  {
    table[i] = a*(round(((sin(2*PI*i/samples))*bitMul))+bitMul);
  }
}

