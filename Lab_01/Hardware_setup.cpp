/*
 * SETUP
 *
 * Hardware_setup.cpp
 *
 *  Created on: Apr 15, 2019
 *
 *      IE-1119 Temas Especiales II: Laboratorio de Introducción a los Sistemas Incrustados
 *
 *      Authors: Dunia, Laura, Sebastian
 */

//*****************************************************************
void PinSetup(void){

    P2->DIR |=  g_iLedLight;

    //Debbugin led (erase)
    P1->DIR |= BIT0; //P1 Red LED - 0x00000001 port P1.0 as an output

    // Set P4.3 for Analog input, disabling the I/O circuit.
    P4->SEL0 = BIT3;
    P4->SEL1 = BIT3;
    P4->DIR &= ~BIT3;

    return;
}
//*****************************************************************
void T32_1_setup(void){

    TIMER32_1->LOAD |= TIMER32_1_COUNT_10ms;
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 |
                         TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    /*TIMER32_CONTROL_PRESCALE_0: 0 stages of prescale, clock is divided by 1 */
    NVIC_SetPriority(T32_INT1_IRQn,1);
    NVIC_EnableIRQ(T32_INT1_IRQn);

    return;
}
//*****************************************************************
void T32_2_setup(void){

    TIMER32_2->LOAD |= TIMER32_2_COUNT_1s;
    TIMER32_2->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_1 |
                         TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    /*TIMER32_CONTROL_PRESCALE_1: 4 stages of prescale, clock is divided by 16 */
    NVIC_SetPriority(T32_INT2_IRQn,1);
    NVIC_EnableIRQ(T32_INT2_IRQn);

    return;
}
//*****************************************************************
void ADC_setup(void){

    //ADC14 Control_0
    ADC14->CTL0 = ADC14_CTL0_PDIV_0 |
                    ADC14_CTL0_SHS_0 |
                    ADC14_CTL0_DIV_7 |
                    ADC14_CTL0_SSEL__MCLK |
                    ADC14_CTL0_SHT0_1 |
                    ADC14_CTL0_ON |
                    ADC14_CTL0_SHP;

    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;

    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC; // set enable
    ADC14->IER0 = ADC14_IER0_IE0;
    NVIC_SetPriority(ADC14_IRQn,1);
    NVIC_EnableIRQ(ADC14_IRQn);

    return;
}
//*****************************************************************
void LightSensorSetup(void){

    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    /* Initialize OPT3001 digital ambient light sensor */
    OPT3001_init();

    __delay_cycles(LIGHT_SENSOR_DELAY);

    return;
}
//*****************************************************************
void ButtonSetup(void){

    // Configure Green and Blue LED
    GPIO_setAsOutputPin( GPIO_PORT_P2 , GPIO_PIN1 ) ;
    GPIO_setAsOutputPin( GPIO_PORT_P2 , GPIO_PIN2 ) ;

    // Set button attached at GPI0_PIN1 as an entry
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1 , GPIO_PIN1 ) ;

    // Set posEdge Interruptions
    GPIO_interruptEdgeSelect( GPIO_PORT_P1 , GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION ) ;
    GPIO_clearInterruptFlag( GPIO_PORT_P1 , GPIO_PIN1 ) ;

    // Enable Port 1 interruptions
    GPIO_enableInterrupt( GPIO_PORT_P1 , GPIO_PIN1 ) ;

    // Enable interruptions at the interruptions vector
    Interrupt_enableInterrupt( INT_PORT1 ) ;

    // Enable global interruptions
    Interrupt_enableMaster() ;

    // Turn LEDS off
    GPIO_setOutputLowOnPin( GPIO_PORT_P2 , GPIO_PIN1 ) ;
    GPIO_setOutputLowOnPin( GPIO_PORT_P2 , GPIO_PIN2 ) ;

    //MAP_Interrupt_setPriority(INT_PORT1,2);
}
//*****************************************************************
void InitialConditionsSetup(void){
    // LEDS off
    P1->OUT &= ~BIT0; //Red LED off
    P2->OUT &= ~BIT0; //Red LED off
    P2->OUT &= ~BIT1; //Green LED off
    P2->OUT &= ~BIT2; //Blue LED off

    g_u8Flags = 0U;

    g_iLampOnCount = 0;
    g_iAudioThreshold = 0;
    g_iAudioSampleCount = 0;

    g_u32AudioSampleSum = 0U;

    g_u16AudioAverage_5s = 0U;
    g_u16AudioAverage_1s = 0U;
    g_u8OverThSampleCount = 0;

    return;
}
//*****************************************************************

void InitialBlink(int i_iBlinkTimes) {

    LED_CTRL->DIR |= g_iLedLight;
    LED_CTRL->OUT |= g_iLedLight;

    for(int i = 0; i < 2*i_iBlinkTimes; i++ ) {
        __delay_cycles(BLINK_DELAY);
        LED_CTRL->OUT ^= g_iLedLight;
    };
    LED_CTRL->OUT ^= g_iLedLight;
    return;
}
//*****************************************************************
void PowerUp(int i_iSystemWatts){

    switch(i_iSystemWatts){
        case 5:{
            g_iLedLight = BLUE_LED;
            break; }
        case 10:{
            g_iLedLight = RED_LED;
            break; }
        case 15:{
            g_iLedLight = BLUE_LED | RED_LED;
            break; }
        default:{
            g_iLedLight = BLUE_LED | RED_LED | GREEN_LED;
            break;}
    }

    //Setup
    PinSetup();


    //Initial Conditions
    InitialConditionsSetup();

    // Initial blink
    InitialBlink(3);

    //Interrupt Setup
    T32_1_setup();
    LightSensorSetup();
    ButtonSetup();
    T32_2_setup();
    ADC_setup();


    // Define light state
    g_fSensorLux = OPT3001_getLux();
    //printf("Inicial:\nLuz %f \t Audio %u \n", g_fSensorLux, g_iAudioThreshold);

    // Set Power-on Flag
    g_u8Flags |= BIT0;
    //printf("%u\n", (g_u8Flags & BIT0));

    return;
}
