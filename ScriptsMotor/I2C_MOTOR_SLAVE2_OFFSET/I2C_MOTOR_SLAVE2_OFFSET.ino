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
#define I2C_ADDRESS 0x06 //Can use script I2C_DEVICE_SCANNER to get address
#define AMPLITUDE 3
#define MAX_SAMPLES 1024

//Variables
int maxSamples = MAX_SAMPLES; //# elements in sine lookup table
int i = 0; //Iterate through wave values for sine wave
int offsetdeg = 97; //120 degrees 
int offset; //Formula: offset degrees * num of samples / 360
int x = 0; //Signal to receive from master
int j = 0; //Iterate through wave values for wave 2

//Example sine lookup table
static int sinewave1[MAX_SAMPLES] = 
{
1536,1545,1554,1564,1573,1583,1592,1601,1611,1620,1630,1639,1649,1658,1667,
1677,1686,1696,1705,1714,1724,1733,1742,1752,1761,1770,1780,1789,1798,1808,
1817,1826,1835,1845,1854,1863,1872,1881,1891,1900,1909,1918,1927,1936,1945,
1955,1964,1973,1982,1991,2000,2009,2018,2027,2035,2044,2053,2062,2071,2080,
2089,2097,2106,2115,2124,2132,2141,2150,2158,2167,2176,2184,2193,2201,2210,
2218,2227,2235,2243,2252,2260,2268,2277,2285,2293,2301,2309,2317,2326,2334,
2342,2350,2358,2366,2374,2381,2389,2397,2405,2413,2420,2428,2436,2443,2451,
2459,2466,2474,2481,2488,2496,2503,2510,2518,2525,2532,2539,2546,2553,2560,
2567,2574,2581,2588,2595,2602,2609,2615,2622,2629,2635,2642,2648,2655,2661,
2668,2674,2680,2687,2693,2699,2705,2711,2717,2723,2729,2735,2741,2747,2753,
2758,2764,2770,2775,2781,2786,2792,2797,2802,2808,2813,2818,2823,2829,2834,
2839,2844,2848,2853,2858,2863,2868,2872,2877,2881,2886,2890,2895,2899,2904,
2908,2912,2916,2920,2924,2928,2932,2936,2940,2944,2947,2951,2955,2958,2962,
2965,2969,2972,2975,2979,2982,2985,2988,2991,2994,2997,3000,3003,3005,3008,
3011,3013,3016,3018,3021,3023,3025,3028,3030,3032,3034,3036,3038,3040,3042,
3044,3045,3047,3049,3050,3052,3053,3055,3056,3057,3059,3060,3061,3062,3063,
3064,3065,3066,3066,3067,3068,3068,3069,3069,3070,3070,3070,3071,3071,3071,
3071,3071,3071,3071,3071,3070,3070,3070,3069,3069,3069,3068,3067,3067,3066,
3065,3064,3063,3062,3061,3060,3059,3058,3057,3055,3054,3053,3051,3050,3048,
3046,3045,3043,3041,3039,3037,3035,3033,3031,3029,3027,3024,3022,3020,3017,
3015,3012,3009,3007,3004,3001,2998,2996,2993,2990,2987,2983,2980,2977,2974,
2970,2967,2964,2960,2957,2953,2949,2946,2942,2938,2934,2930,2926,2922,2918,
2914,2910,2906,2901,2897,2893,2888,2884,2879,2875,2870,2865,2861,2856,2851,
2846,2841,2836,2831,2826,2821,2816,2810,2805,2800,2794,2789,2784,2778,2772,
2767,2761,2755,2750,2744,2738,2732,2726,2720,2714,2708,2702,2696,2690,2684,
2677,2671,2665,2658,2652,2645,2639,2632,2625,2619,2612,2605,2599,2592,2585,
2578,2571,2564,2557,2550,2543,2536,2529,2521,2514,2507,2499,2492,2485,2477,
2470,2462,2455,2447,2440,2432,2424,2417,2409,2401,2393,2385,2378,2370,2362,
2354,2346,2338,2330,2322,2313,2305,2297,2289,2281,2272,2264,2256,2247,2239,
2231,2222,2214,2205,2197,2188,2180,2171,2163,2154,2145,2137,2128,2119,2111,
2102,2093,2084,2075,2067,2058,2049,2040,2031,2022,2013,2004,1995,1986,1977,
1968,1959,1950,1941,1932,1923,1914,1904,1895,1886,1877,1868,1858,1849,1840,
1831,1821,1812,1803,1794,1784,1775,1766,1756,1747,1738,1728,1719,1710,1700,
1691,1681,1672,1663,1653,1644,1634,1625,1616,1606,1597,1587,1578,1569,1559,
1550,1540,1531,1521,1512,1502,1493,1484,1474,1465,1455,1446,1437,1427,1418,
1408,1399,1390,1380,1371,1361,1352,1343,1333,1324,1315,1305,1296,1287,1277,
1268,1259,1250,1240,1231,1222,1213,1203,1194,1185,1176,1167,1157,1148,1139,
1130,1121,1112,1103,1094,1085,1076,1067,1058,1049,1040,1031,1022,1013,1004,
996,987,978,969,960,952,943,934,926,917,908,900,891,883,874,
866,857,849,840,832,824,815,807,799,790,782,774,766,758,749,
741,733,725,717,709,701,693,686,678,670,662,654,647,639,631,
624,616,609,601,594,586,579,572,564,557,550,542,535,528,521,
514,507,500,493,486,479,472,466,459,452,446,439,432,426,419,
413,406,400,394,387,381,375,369,363,357,351,345,339,333,327,
321,316,310,304,299,293,287,282,277,271,266,261,255,250,245,
240,235,230,225,220,215,210,206,201,196,192,187,183,178,174,
170,165,161,157,153,149,145,141,137,133,129,125,122,118,114,
111,107,104,101,97,94,91,88,84,81,78,75,73,70,67,
64,62,59,56,54,51,49,47,44,42,40,38,36,34,32,
30,28,26,25,23,21,20,18,17,16,14,13,12,11,10,
9,8,7,6,5,4,4,3,2,2,2,1,1,1,0,
0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,
5,5,6,7,8,9,10,11,12,14,15,16,18,19,21,
22,24,26,27,29,31,33,35,37,39,41,43,46,48,50,
53,55,58,60,63,66,68,71,74,77,80,83,86,89,92,
96,99,102,106,109,113,116,120,124,127,131,135,139,143,147,
151,155,159,163,167,172,176,181,185,190,194,199,203,208,213,
218,223,227,232,237,242,248,253,258,263,269,274,279,285,290,
296,301,307,313,318,324,330,336,342,348,354,360,366,372,378,
384,391,397,403,410,416,423,429,436,442,449,456,462,469,476,
483,490,497,504,511,518,525,532,539,546,553,561,568,575,583,
590,597,605,612,620,628,635,643,651,658,666,674,682,690,697,
705,713,721,729,737,745,754,762,770,778,786,794,803,811,819,
828,836,844,853,861,870,878,887,895,904,913,921,930,939,947,
956,965,974,982,991,1000,1009,1018,1027,1036,1044,1053,1062,1071,1080,
1089,1098,1107,1116,1126,1135,1144,1153,1162,1171,1180,1190,1199,1208,1217,
1226,1236,1245,1254,1263,1273,1282,1291,1301,1310,1319,1329,1338,1347,1357,
1366,1375,1385,1394,1404,1413,1422,1432,1441,1451,1460,1470,1479,1488,1498,
1507,1517,1526,1536
};

static int sinewave2[MAX_SAMPLES]; //2nd wave starts at offset

//Method to configure Arduino
void setup() 
{
   //Start the I2C Bus as Slave on address 0x04
   Wire.begin(I2C_ADDRESS); 

   pinMode(DAC0, OUTPUT);  
   analogWriteResolution(DAC_RESOLUTION); //Set up resolution
   offset = offsetdeg*maxSamples/360; //Calculate offset
   generateSineShifted(offset, sinewave2); //Copy values from lookup table starting at offset on sinewave2
}

//Method to set the signal to what was transfered by master
void receiveEvent(int bytes) 
{
  x = Wire.read(); //Read one character from the I2C master and assign to x
 
}

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

//Method to loop indefinetely
void loop() 
{
    Wire.onReceive(receiveEvent); //Receive signal from master
      
    if (x == 1) //If the signal was true, start generating wave
    {
      i = (i+1)%maxSamples; // Next element, and if reached last, go back to 0
      analogWrite(DAC0, sinewave2[i]); //Write every element of sinewave
    }    
}
