/*
 * SMART LAMP
 *
 * main.cpp
 *
 *  Created on: Apr 15, 2019
 *
 *      IE-1119 Temas Especiales II: Laboratorio de Introducción a los Sistemas Incrustados
 *
 *      Authors: Dunia, Laura, Sebastian
 */

#include "main.hpp"


int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   /** Stop watchdog timer **/
    Power_up(SYSTEM_WATTS);                 /** Power-up **/

    while (1) {

        //--------- Verify Rooms light level ---------
        g_fSensorLux = OPT3001_getLux();

        if(g_fSensorLux > LightThreshold){
            //if there is light (day light/external lamp on)
            g_u8Flags &= ~BIT2; // CLEAR Under Light Threshold Flag
        }
        else{
            g_u8Flags |= BIT2;  // SET Under Light Threshold Flag
        }

        //--------- Verify Lamp On Conditions ---------
        /*if Flags:
         * g_u8Flags BIT3: Over Audio Threshold  = true
         * g_u8Flags BIT2: Under Light Threshold = true
         * g_u8Flags BIT1: Lamp Button Activated = false
         * g_u8Flags BIT0: Power On  = true
         * */

        if( (g_u8Flags & BIT1) == 0U){
            if( (g_u8Flags & BIT2) == BIT2){
                if( (g_u8Flags & BIT3) == BIT3){

                    //LED_CTRL->OUT = BIT1;
                    LED_CTRL->OUT = g_LED_LIGHT; //Turn LED on
                    g_iLampOnCount = LampOnInterval;
                }
            }
            else{
                // if lamp is not activated by button and Under Light Threshold Flag is false
                //LED_CTRL->OUT &= ~BIT1;
                LED_CTRL->OUT &= ~g_LED_LIGHT; //Turn LED off
            }
        }
    }
    return 0;
}





