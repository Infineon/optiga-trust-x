/**
* MIT License
*
* Copyright (c) 2018 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
*
* \file
*
* \brief This file implements the platform abstraction layer APIs for timer.
*
* \ingroup  grPAL
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "optiga/pal/pal_os_timer.h"
#include "nrf_delay.h"
#include "nrf_rtc.h"
#include "nrf_drv_rtc.h"

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
// must be same as pal_os_event.c
#define RTC_CLOCK_FREQUENCY 32768
// Set the prescaler to approximately get 0.25 ms intervals
// 32768Hz/8 = 4096 Hz -> 0.2441us
// it's a 24bit counter, so it will overflow every ~68min
#define RTC_PRESCALER 8
#define RTC_TICK_FREQ (RTC_CLOCK_FREQUENCY/RTC_PRESCALER)

/// @cond hidden 
/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
// Defined in pal_os_event.c
// needed to read the tick value
extern const nrf_drv_rtc_t rtc2;

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/
/// @endcond

/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
/**
* Get the current time in milliseconds<br>
*
*
* \retval  uint32_t time in milliseconds
*/
uint32_t pal_os_timer_get_time_in_milliseconds(void)
{
    return nrf_drv_rtc_counter_get(&rtc2)*1000/RTC_TICK_FREQ;
}

/**
* Function to wait or delay until the given milliseconds time
* 
* \param[in] milliseconds Delay value in milliseconds
*
*/
void pal_os_timer_delay_in_milliseconds(uint16_t milliseconds)
{
    nrf_delay_ms(milliseconds);
}

/**
* @}
*/

