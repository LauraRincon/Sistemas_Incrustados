/*
 * SETUP
 *
 * Hardware_setup.hpp
 *
 *  Created on: Apr 15, 2019
 *
 *      IE-1119 Temas Especiales II: Laboratorio de Introducción a los Sistemas Incrustados
 *
 *      Authors: Dunia, Laura, Sebastian
 */

#ifndef HARDWARE_SETUP_HPP_
#define HARDWARE_SETUP_HPP_

#define TIMER32_1_COUNT_1ms  0x00000BB7     //~1ms with 3MHz clock and clock divided by 1
#define TIMER32_1_COUNT_5ms  0x00003A97     //~5ms with 3MHz clock and clock divided by 1
#define TIMER32_1_COUNT_10ms  0x0000752F    //~10ms with 3MHz clock and clock divided by 1
#define TIMER32_1_COUNT_1s  0x002DC6BF      //~1s with 3MHz clock and clock divided by 1

#define TIMER32_2_COUNT_1s 0x0002DC6B   //~1s with 3MHz clock and clock divided by 16
#define TIMER32_2_COUNT_5s 0x000E4E1B   //~5s with 3MHz clock and clock divided by 16
#define TIMER32_2_COUNT_60s 0x00ABA94F  //~60s with 3MHz clock and clock divided by 16
#define TIMER32_2_COUNT_30s 0x0055D4A7  //~30s with 3MHz clock and clock divided by 16

//#define BLINK_DELAY 0x16E360             //~0.5s delay for the initial blink
#define BLINK_DELAY 0x2DC6C0            //~1s delay for the initial blink
#define LIGHT_SENSOR_DELAY 100000       //~33ms delay for light sensor configure

//#define MASK(x) (1<< (x))
#define BLUE_LED 0x04
#define GREEN_LED 0x02
#define RED_LED 0x01

int g_LED_LIGHT;

void Pin_setup(void);
void ADC_setup(void);
void T32_1_setup(void);
void T32_2_setup(void);
void Button_setup(void);
void LightSensor_setup(void);

void InitialConditions_setup(void);
void Initial_blink(int,int);
void Power_up(int);


#endif /* HARDWARE_SETUP_HPP_ */
