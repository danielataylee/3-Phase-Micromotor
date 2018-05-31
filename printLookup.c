//This script prints the values in the lookup table 

#include <stdio.h>
#include <math.h>

//Constants
#define SAMPLES 1000
#define OFFDEG 0
#define PI 3.14159265359

int i=0;
int j=0;
int offset = OFFDEG*SAMPLES/360; //Calculate offset position -> Formula: offset degrees * num of samples / 360
int bitMul = (1<<12)/2; //1<<12 Is equivalent to 2^12 = 4096

//Lookup table
static int wave[SAMPLES]; 

//Method to generate sine lookup table
void generateSineTable(int off, int * table, int s)
{
  for (i = off ; i < (s + off) ; i++)
    table[j++] = (round(((sin(2*PI*i/s))*(bitMul-1)))+bitMul);
  //Reset variables
  i=0;
  j=0;
}

int main()
{
    
    //Call method to generate sine lookup table
    generateSineTable(offset, wave, SAMPLES); 
   
    for (i=0;i<SAMPLES;i++)
    {
        printf("%d\n", wave[i]);
    }
    return 0;
}





















//This script prints the values in the lookup table 

#include <stdio.h>
#include <math.h>

//Constants
#define SAMPLES 1000
#define OFFDEG 0
#define PI 3.14159265359

int i=0;
int j=0;
int samples=0;
int offset = OFFDEG*SAMPLES/360; //Calculate offset position -> Formula: offset degrees * num of samples / 360
int bitMul = (1<<12)/2; //1<<12 Is equivalent to 2^12 = 4096

//Lookup table
static int * wave; 

//Method to generate sine lookup table
void generateSineTable(int off, int * table, int s)
{
  for (i = off ; i < (s + off) ; i++)
    *table++ = (round(((sin(2*PI*i/s))*(bitMul-1)))+bitMul);
  //Reset variables
  i=0;
  j=0;
}

int main()
{
    samples=1000;
    wave = malloc(sizeof(int)*samples);
    
    //Call method to generate sine lookup table
    generateSineTable(offset, wave, SAMPLES); 
   
    for (i=0;i<SAMPLES;i++)
    {
        printf("%d\n", wave[i]);
    }
    return 0;
}
