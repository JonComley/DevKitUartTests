/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "adc.h"
#include "lcd.h"
#include "timer_1ms.h"
#include "buttons.h"
#include "leds.h"
#include "rtcc.h"
 
#include "io_mapping.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************
extern void SYS_Initialize ( void ) ;
static void TimerEventHandler( void );

static RTCC_DATETIME time;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    uint16_t adcResult;
    
    /* Call the System Initialize routine*/
    SYS_Initialize ( );
    
    /* To determine how the LED and Buttons are mapped to the actual board
     * features, please see io_mapping.h. */
    LED_Enable ( LED_BLINK_ALIVE );
    LED_Enable ( LED_BUTTON_PRESSED );

    BUTTON_Enable ( BUTTON_DEMO );
    
    /* Get a timer event once every 100ms for the blink alive. */
    TIMER_SetConfiguration ( TIMER_CONFIGURATION_1MS );
    TIMER_RequestTick( &TimerEventHandler, 100 );
    
    /* The TIMER_1MS configuration should come before the RTCC initialization as
     * there are some processor modules that require the TIMER_1MS module to be
     * configured before the RTCC module, as the RTCC module is emulated using
     * the TIMER_1MS module. */
    time.bcdFormat = false;
    RTCC_BuildTimeGet( &time );
    RTCC_Initialize( &time );

    ADC_SetConfiguration ( ADC_CONFIGURATION_DEFAULT );
    ADC_ChannelEnable ( ADC_CHANNEL_POTENTIOMETER );
    
    /* Clear the screen */
    printf( "\f" );   
    
    while ( 1 )
    {
        adcResult = ADC_Read10bit( ADC_CHANNEL_POTENTIOMETER );

        RTCC_TimeGet( &time );
        
        printf( "Time %02d:%02d:%02d   Pot = %4d\r\n", 
                time.hour, 
                time.minute, 
                time.second, 
                adcResult
              );

        /* To determine how the LED and Buttons are mapped to the actual board
         * features, please see io_mapping.h. */
        if(BUTTON_IsPressed( BUTTON_DEMO ) == true)
        {
            LED_On( LED_BUTTON_PRESSED );
        }
        else
        {
            LED_Off( LED_BUTTON_PRESSED );
        }
    }
}

static void TimerEventHandler(void)
{    
    LED_Toggle( LED_BLINK_ALIVE );
}
