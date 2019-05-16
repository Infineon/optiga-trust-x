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
extern "C" {
	#include "optiga/pal/pal_os_timer.h"
}
#include "mbed.h"
/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
 
/// @cond hidden 
/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
//Serial pcy(SERIAL_TX, SERIAL_RX);
/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/


/// @endcond
/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
//extern void pal_os_event_trigger_registered_callback();
/**
* Get the current time in milliseconds<br>
*
*
* \retval  uint32_t time in milliseconds
*/
uint32_t pal_os_timer_get_time_in_milliseconds(void)
{
    return (us_ticker_read()/1000); //xTaskGetTickCount();
}

/**
* Waits or delays until the given milliseconds time
* 
* \param[in] milliseconds Delay value in milliseconds
*
*/
void pal_os_timer_delay_in_milliseconds(uint16_t milliseconds)
{
    /* const TickType_t xDelay = milliseconds / portTICK_PERIOD_MS;
	vTaskDelay( xDelay );
	*/
	//pcy.printf("Waiting %d\r\n", milliseconds);
	//pal_os_event_trigger_registered_callback();
	wait_ms(milliseconds);
}

/**
* @}
*/

