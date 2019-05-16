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
* \brief This file implements the platform abstraction layer APIs for os event/scheduler.
*
* \ingroup  grPAL
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
extern "C" {
	#include "optiga/pal/pal_os_event.h"
	#include "optiga/pal/pal.h"
}
#include "mbed.h"
/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/

 Serial pcx(SERIAL_TX, SERIAL_RX);
 /*
 DigitalOut dbg(PC_12, 0);
 DigitalOut dbg1(PC_10, 0);
 DigitalOut dbg2(PC_8, 0);
 DigitalOut dbg3(PC_6, 0);
 DigitalOut dbg4(PC_5, 0);
 */
/// @cond hidden 
/// Callback function when timer elapses
static volatile register_callback callback_registered = NULL;
/// Pointer to store upper layer callback context (For example: Ifx i2c context)
static void * callback_ctx;
static int timer_expire_in = 0;
Ticker event_timer;

pal_status_t pal_os_event_init(void)
{
	return PAL_STATUS_SUCCESS;
}

void pal_os_event_trigger_registered_callback()
{
    register_callback callback;

	//pcx.printf("Exp = %d, \n Time: %d\n\r", timer_expire_in, event_timer.read_us());
    if (callback_registered)
    {
        callback = callback_registered;
        callback_registered = NULL;
        timer_expire_in = 0;
        callback((void*)callback_ctx);
    }
}
/// @endcond

/**
* Platform specific event call back registration function to trigger once when timer expires.
* <br>
*
* <b>API Details:</b>
*         This function registers the callback function supplied by the caller.<br>
*         It triggers a timer with the supplied time interval in microseconds.<br>
*         Once the timer expires, the registered callback function gets called.<br>
* 
* \param[in] callback              Callback function pointer
* \param[in] callback_args         Callback arguments
* \param[in] time_us               time in micro seconds to trigger the call back
*
*/
void pal_os_event_register_callback_oneshot(register_callback callback, 
                                            void * callback_args, 
                                            uint32_t time_us)
{
    callback_registered = callback;
	callback_ctx = callback_args;
	event_timer.attach_us(&pal_os_event_trigger_registered_callback, time_us);
}

/**
* @}
*/

