// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

// Except where otherwise stated.

#include "tastefullynude.h"

// Creating the task
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Watchdog write protect enable/disable, etc
#include "hal/wdt_hal.h"

//
// Disabling the watchdog
// This section lifted from gdbstub.c, almost verbatim
//

// If this breaks in the future, the alternative is to just
// go into c:\idf\components\esp_gdbsbub\include\esp_gdbstub.h
// and add an externally visible declaration for void disable_all_wdts(void);

static wdt_hal_context_t rtc_wdt_ctx = {.inst = WDT_RWDT, .rwdt_dev = &RTCCNTL};
static wdt_hal_context_t wdt0_context = {.inst = WDT_MWDT0, .mwdt_dev = &TIMERG0};
static wdt_hal_context_t wdt1_context = {.inst = WDT_MWDT1, .mwdt_dev = &TIMERG1};

static bool wdt0_context_enabled = false;
static bool wdt1_context_enabled = false;
static bool rtc_wdt_ctx_enabled = false;

static inline void DisableWatchdog(void)
{
    wdt0_context_enabled = wdt_hal_is_enabled(&wdt0_context);
    wdt1_context_enabled = wdt_hal_is_enabled(&wdt1_context);
    rtc_wdt_ctx_enabled = wdt_hal_is_enabled(&rtc_wdt_ctx);

    //Task WDT is the Main Watchdog Timer of Timer Group 0
    if (true == wdt0_context_enabled) {
        wdt_hal_write_protect_disable(&wdt0_context);
        wdt_hal_disable(&wdt0_context);
        wdt_hal_feed(&wdt0_context);
        wdt_hal_write_protect_enable(&wdt0_context);
    }

    //Interupt WDT is the Main Watchdog Timer of Timer Group 1
    if (true == wdt1_context_enabled) {
        wdt_hal_write_protect_disable(&wdt1_context);
        wdt_hal_disable(&wdt1_context);
        wdt_hal_feed(&wdt1_context);
        wdt_hal_write_protect_enable(&wdt1_context);
    }
    if (true == rtc_wdt_ctx_enabled) {
        wdt_hal_write_protect_disable(&rtc_wdt_ctx);
        wdt_hal_disable(&rtc_wdt_ctx);
        wdt_hal_feed(&rtc_wdt_ctx);
        wdt_hal_write_protect_enable(&rtc_wdt_ctx);
    }
}

//
// Disabling the interruptions on Core 1
//

static void DisableInterruptions(){

    // Essentially an expansion of ETS_WDT_INTR_DISABLE()
    // as you'll find it in idf/esp_rom/include/esp32*/rom/ets_sys.h
    ets_isr_mask( 1 << 6 );

}

//
// And Some setup stuff
//

// Little task to be sure we're on whichever core
void RunAppOnCore_Task( void * inFunction ){

    AppCoreFunction func = (AppCoreFunction)inFunction;
    
    DisableWatchdog();
    DisableInterruptions();

    func();

    vTaskDelete(NULL);

}

void RunOnAppCore( AppCoreFunction inFunction ){

    // Pin a task to guarantee the correct core:
    xTaskCreatePinnedToCore(RunAppOnCore_Task, "RunAppOnCore_Task", 4096, (void*)inFunction, 10, NULL, 1);

}