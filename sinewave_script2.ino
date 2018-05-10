//This script generates 3 sine waves at different phases at offset, need to figure out how to control other DAC

//Constants
#define MAX_SAMPLES 120 //There are 120 elements in array
#define DAC_RESOLUTION 12

//Variables
int i = 0; //Iterate through wave values for wave 1
int j = 0; //Iterate through wave values for wave 2
int offsetdeg = 120; //120 degrees 
int offset; ////Formula: offset degrees * num of samples / 360

//sine wave lookup table
static int sinewave1[MAX_SAMPLES] = 
{
    0x7ff, 0x86a, 0x8d5, 0x93f, 0x9a9, 0xa11, 0xa78, 0xadd, 0xb40, 0xba1,
    0xbff, 0xc5a, 0xcb2, 0xd08, 0xd59, 0xda7, 0xdf1, 0xe36, 0xe77, 0xeb4,
    0xeec, 0xf1f, 0xf4d, 0xf77, 0xf9a, 0xfb9, 0xfd2, 0xfe5, 0xff3, 0xffc,
    0xfff, 0xffc, 0xff3, 0xfe5, 0xfd2, 0xfb9, 0xf9a, 0xf77, 0xf4d, 0xf1f,
    0xeec, 0xeb4, 0xe77, 0xe36, 0xdf1, 0xda7, 0xd59, 0xd08, 0xcb2, 0xc5a,
    0xbff, 0xba1, 0xb40, 0xadd, 0xa78, 0xa11, 0x9a9, 0x93f, 0x8d5, 0x86a,
    0x7ff, 0x794, 0x729, 0x6bf, 0x655, 0x5ed, 0x586, 0x521, 0x4be, 0x45d,
    0x3ff, 0x3a4, 0x34c, 0x2f6, 0x2a5, 0x257, 0x20d, 0x1c8, 0x187, 0x14a,
    0x112, 0xdf, 0xb1, 0x87, 0x64, 0x45, 0x2c, 0x19, 0xb, 0x2,
    0x0, 0x2, 0xb, 0x19, 0x2c, 0x45, 0x64, 0x87, 0xb1, 0xdf,
    0x112, 0x14a, 0x187, 0x1c8, 0x20d, 0x257, 0x2a5, 0x2f6, 0x34c, 0x3a4,
    0x3ff, 0x45d, 0x4be, 0x521, 0x586, 0x5ed, 0x655, 0x6bf, 0x729, 0x794
};

static int sinewave2[MAX_SAMPLES]; //2nd wave starts at offset
static int sinewave3[MAX_SAMPLES]; //3rd wave starts at 2*offset

void setup() 
{
  //Set DACs as outputs
  pinMode(DAC0, OUTPUT);  
  pinMode(DAC1, OUTPUT);  
  analogWriteResolution(DAC_RESOLUTION); //Set up resolution
  offset = offsetdeg*MAX_SAMPLES/360; //Calculate offset

  generateSineShifted(offset, sinewave2); //Copy values from lookup table starting at offset on sinewave2
  generateSineShifted(offset*2, sinewave3); //Copy values from lookup table starting at offsetX2 on sinewave3
 
}

//This method fills an array to generate a sine wave starting at offset offs
void generateSineShifted(int offs, int * wave)
{
  //Copy values from lookup table starting at offset on sinewave
  j=0;
  for (i=offs ; i< MAX_SAMPLES ; i++) //Start from offset of i until max samples of i
    wave[j++] = sinewave1[i];
  i = 0; //Reset i
  for (j=j ; j < MAX_SAMPLES ; j++) //Start from where left off on sinewave2 and copy values of sinewave1 from 0 to offset
    wave[j] = sinewave1[i++];
  i=0; //Reset i
}

void loop() 
{
    i++; //Next element
    if(i == MAX_SAMPLES) //If already reached last element, start again at first
      i = 0;   
  
    analogWrite(DAC0, sinewave1[i]); //Write every element of sinewave1
    analogWrite(DAC1, sinewave2[i]); //Write every element of sinewave2 (2nd wave phase shifted by offset)

    //analogWrite(OTHER DAC HERE!, sinewave3[i]); //Write every element of sinewave3 (3rd wave phase shifted by offsetX2)
}
