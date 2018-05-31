# 3-Phase-Micromotor

Date: May 2018 <br /><br />
Organization: BCCRC Imaging Unit <br /><br />
Authors:     Varalee Chinbomsom and Daniela Tay Lee Sanchez <br /><br />
Purpose:     To drive a dual beam micromotor catheter (Namiki Precision). <br /><br />
Description: The motor requires 3 sine waves so it needs 3 Arduino Due, where each uses its own DAC to output a sine wave. The 3 waves have a phase shift of 120 degrees. The number of samples in the lookup table is set to 1000, so it ramps up to 100 Hz. The motor ramps up by 10 Hz each time and completes 100 cycles (complete sine wave) on each step. The maximum value in the table is 4095 because it uses a 12 bit DAC. Therefore, the amplitude is approx 2.7 V. The output can be reset/resynced with a switch and the specifications such as the number of samples can be changed by setting different values in the scripts. 
