/*
 * INTERRUPTIONS
 *
 * Interruptions_handler.hpp
 *
 *  Created on: Apr 15, 2019
 *
 *      IE-1119 Temas Especiales II: Laboratorio de Introducción a los Sistemas Incrustados
 *
 *      Authors: Dunia, Laura, Sebastian
 */

//#include "Hardware_setup.hpp"

#ifndef INTERRUPTIONS_HANDLER_HPP_
#define INTERRUPTIONS_HANDLER_HPP_

#define LampOnInterval 5

uint8_t g_u8OverThSampleCount;

uint16_t g_u16Adc14Result[6];   //Storage last 5s audio samples
uint16_t g_u16AudioAverage_5s;  //Storage last 5s average audio
uint16_t g_u16AudioAverage_1s;
uint32_t g_u32AudioSampleSum;

int g_iAudioThreshold;
int g_iAudioSampleCount;



#endif /* INTERRUPTIONS_HANDLER_HPP_ */
