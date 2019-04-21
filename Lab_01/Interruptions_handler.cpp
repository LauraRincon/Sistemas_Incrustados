/*
 * INTERRUPTIONS
 *
 * Interruptions_handler.cpp
 *
 *  Created on: Apr 15, 2019
 *
 *      IE-1119 Temas Especiales II: Laboratorio de Introducción a los Sistemas Incrustados
 *
 *      Authors: Dunia, Laura, Sebastian
 */

//#include "Interruptions_handler.hpp"


extern "C" {

//----------------------------------------------------------------
    void T32_INT1_IRQHandler(void) {

        // Generates an interrupt every 10ms:
        __disable_irq();
        TIMER32_1->INTCLR = 0U;

        // ADC14 starts
        ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC;

        __enable_irq();

        return;
    }

//----------------------------------------------------------------
    void T32_INT2_IRQHandler(void) {

        __disable_irq();

        TIMER32_2->INTCLR = 0U; //Clear interrup Flag

        int l_iAdc14ResultIndex;

        // Comparison
        //printf("Cont Sample %i  Cont Picos %i \n", g_iAudioSampleCount, g_u8OverThSampleCount);
        if (g_u8OverThSampleCount >= (g_iAudioSampleCount*0.1) ){
            P1->OUT |= BIT0;
            g_u8Flags |= BIT3;
        }
        else{
            P1->OUT &= ~BIT0;
            g_u8Flags &= ~BIT3;
        }
        //printf("Over Audio TH %u \n", (g_u8Flags & BIT2) );
        g_u8OverThSampleCount = 0;

        // 1s Audio sample average
        g_u16AudioAverage_1s = g_u32AudioSampleSum / g_iAudioSampleCount;
        //printf("1sAverage %u \n", g_u16AudioAverage_1s);

        // Reset
        g_u32AudioSampleSum = 0U;
        g_iAudioSampleCount = 0;

        g_u16Adc14Result[5] = g_u16AudioAverage_1s;

        for (l_iAdc14ResultIndex = 0; l_iAdc14ResultIndex<5; l_iAdc14ResultIndex++){

            g_u16AudioAverage_5s += g_u16Adc14Result[l_iAdc14ResultIndex];
            g_u16Adc14Result[l_iAdc14ResultIndex] = g_u16Adc14Result[l_iAdc14ResultIndex+1];

            if (l_iAdc14ResultIndex == 4){
                g_u16AudioAverage_5s /= 5;
                g_iAudioThreshold = ((g_u16AudioAverage_5s*10)/100) + g_u16AudioAverage_5s;
                //printf("5sAverage %u \t TH %u \n\n", g_u16AudioAverage_5s, g_iAudioThreshold);
                g_u16AudioAverage_5s = 0U;
            }
        }

        // Lamp On during LampOnInterval seconds
        if (g_iLampOnCount > 0) {
            g_iLampOnCount --;
        }
        else if ( (g_iLampOnCount == 0) ) {
            LED_CTRL->OUT &= ~g_iLedLight; //Turn off light
            g_u8Flags &= ~BIT1;
        }

        __enable_irq();

        return;
    }

//----------------------------------------------------------------
    void ADC14_IRQHandler(void) {

        /*ADC14 para obtener nivel de sonido en la habitacion*/
        __disable_irq();

        //printf("1sAverage %u  l_iAudioThreshold %u \n\n", g_u16AudioAverage_1s, g_iAudioThreshold);
        if( (g_u8Flags & BIT0) == BIT0){

            if(ADC14->MEM[0] >= g_iAudioThreshold){
                g_u8OverThSampleCount ++;
            }
        }

        //printf("SAMPLE %u\n", ADC14->MEM[0]);
        g_u32AudioSampleSum += ADC14->MEM[0];
        g_iAudioSampleCount ++;

        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;
        __enable_irq();

        return;
    }

//----------------------------------------------------------------
    void PORT1_IRQHandler( void ) {

        uint32_t l_u32GpioStatus;
        l_u32GpioStatus = GPIO_getEnabledInterruptStatus ( GPIO_PORT_P1 ) ;

        if ( l_u32GpioStatus & GPIO_PIN1 ) {

            // Toggle the state of light
            GPIO_toggleOutputOnPin( GPIO_PORT_P2 , g_iLedLight ) ;

            if(P2->OUT != 0){
                g_u8Flags |= BIT1;
                g_iLampOnCount = LAMP_ON_INTERVAL;}

            GPIO_clearInterruptFlag( GPIO_PORT_P1 , l_u32GpioStatus ) ;
        }
        //printf("flag %x \n", (g_u8Flags & BIT1));
    }
}
