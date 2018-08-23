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
* \brief This file implements the platform abstraction layer APIs for random seed generation.
*
* \ingroup  grPAL
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include <DAVE.h>
#include "optiga/pal/pal_os_timer.h"

/// @cond hidden
/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
///Counter to record random count
static uint32_t g_random_count;

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/
/**
* ISR handler for counting ticks for random count
*
*/
void random_tick_timer_isr(void)
{
    //14 bit counter
    g_random_count = ((g_random_count+1)%(0x4000));
}

/// @endcond
/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
/**
* Get the random counter value.
*
*
* \retval  uint32_t 32 bit random counter value
*/
uint32_t pal_os_random_get_counter(void)
{
    uint32_t millisec_start_count;
    uint32_t millisec_current_count;

    millisec_start_count = pal_os_timer_get_time_in_milliseconds();
    do
    {
        millisec_current_count = pal_os_timer_get_time_in_milliseconds();
        if(millisec_current_count > millisec_start_count)
        {
            break;
        }
    } while(1);

    //stop timer
    (void)TIMER_Stop(&random_timer);

    //Return the current random timer value
    return g_random_count;
}

/**
* @}
*/
