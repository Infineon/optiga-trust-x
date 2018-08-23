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
#include <DAVE.h>
#include "optiga/pal/pal_os_timer.h"

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
 
/// @cond hidden 
/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
 
/* Counter for 1 millisecond ticks */
static volatile uint32_t g_tick_count = 0;

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/

/**
* ISR handler for counting 1 milliseconds ticks
*/
void delay_timer_isr(void)
{
    TIMER_ClearEvent(&tick_timer);
    (void)TIMER_Clear(&tick_timer);
    g_tick_count += 1U;
}

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
    return (g_tick_count);
}

/**
* Waits or delays until the given milliseconds time
* 
* \param[in] milliseconds Delay value in milliseconds
*
*/
void pal_os_timer_delay_in_milliseconds(uint16_t milliseconds)
{
    uint32_t start_time;
    uint32_t current_time;

    start_time = pal_os_timer_get_time_in_milliseconds();
    current_time = start_time;
    while ((current_time - start_time) <= (uint32_t)milliseconds)
    {
        current_time = pal_os_timer_get_time_in_milliseconds();
    }
}

/**
* @}
*/

