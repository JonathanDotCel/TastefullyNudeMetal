#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// The bit that does the thing
#include "tastefullynude.h"

// Flash an LED on the Pro CPU / Core 0,
// to demonstrate that it's still running.
// Has to be a green one or it won't work.
#define LEDPIN_PRO 13
static void FlashLED_ProCPU(){

    uint8_t ledState = 1;

    gpio_reset_pin( LEDPIN_PRO );
    gpio_set_direction( LEDPIN_PRO, GPIO_MODE_OUTPUT );

    while ( 1 ){
        
        //ets_printf( "Pro Blip! Core %d\n", xPortGetCoreID() );
        gpio_set_level(LEDPIN_PRO, ledState);
        vTaskDelay( pdMS_TO_TICKS( 100 ) );
        ledState = !ledState;

    }
    
}

// Output a square wave on App CPU / Core 1
#define SQUAREPIN 21
static void SquareWave_AppCPU(){

    printf( "Doin a sqaure wave on core %d...\n", xPortGetCoreID() );

    gpio_reset_pin( SQUAREPIN );
    gpio_set_direction( SQUAREPIN, GPIO_MODE_OUTPUT );
    gpio_set_level( SQUAREPIN, 1 );

    uint8_t blipBloop = 0;
    
    while ( 1 ){
        gpio_set_level( SQUAREPIN, blipBloop );
        blipBloop = !blipBloop;
    }

}

void app_main(void) { 
    
    printf("app_main()\n"); 
    
    // Calling this directly would make
    // the watchdog angery.
    RunOnAppCore( SquareWave_AppCPU );

    FlashLED_ProCPU();
    
}
