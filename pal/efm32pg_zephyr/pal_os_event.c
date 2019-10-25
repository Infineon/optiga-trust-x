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
/* Optiga based includes */
#include <optiga/pal/pal_os_event.h>
#include <optiga/pal/pal.h>

/* Zephyr based includes */
#include <kernel.h>

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define TIMER_CALLBACK_STACK_SIZE    1000
#define CALLBACK_HANDLER_THRD_PRIO   1

/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
typedef struct callbacks {
    volatile register_callback func; /* Callback function when timer elapses */
    void * args;            /* Pointer to store upper layer callback context */
} pal_os_event_clbs_t;

static void timer_callback_func();
static void callback_handler_thrd(void *unused1, void *unused2, void *unused3);

static pal_os_event_clbs_t clb_ctx_0;

/* on boot kernel objects defines */
K_SEM_DEFINE(timer_lock, 0, 1);     /* create semaphore for timer callback */
K_TIMER_DEFINE(timer_callback_ctx, timer_callback_func, NULL); /* timer */
K_THREAD_STACK_DEFINE(callback_stack_area, TIMER_CALLBACK_STACK_SIZE);
K_THREAD_DEFINE(clb_tid, TIMER_CALLBACK_STACK_SIZE,
        callback_handler_thrd, NULL, NULL, NULL,
        CALLBACK_HANDLER_THRD_PRIO, 0, K_NO_WAIT);

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/
/**
*  Timer callback handler.
*
*  This get called from the TIMER elapse event.<br>
*  Once the timer expires, the registered callback funtion gets called from the timer event handler, if
*  the call back is not NULL.<br>
*
*\param[in] unused arguments
*/
static void callback_handler_thrd(void *unused1, void *unused2, void *unused3)
{
    ARG_UNUSED(unused1);
    ARG_UNUSED(unused2);
    ARG_UNUSED(unused3);

    register_callback func = NULL;
    void *func_args = NULL;

    while (1) {
        k_sem_take(&timer_lock, K_FOREVER);

        if(clb_ctx_0.func) {
            func = clb_ctx_0.func;
            func_args = clb_ctx_0.args;
            func((void*)func_args);
        }
    }
}

static void timer_callback_func()
{
    k_sem_give(&timer_lock);
}

/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
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
*/
void pal_os_event_register_callback_oneshot(register_callback callback, 
                                            void* callback_args, 
                                            uint32_t time_us)
{
    if (time_us < 1000) {
        time_us = 1000;
     }

    clb_ctx_0.func = callback;
    clb_ctx_0.args = callback_args;

    k_timer_start(&timer_callback_ctx, time_us/1000, 0);
}

/**
* Platform specific task delay function.
* <br>
*
* <b>API Details:</b>
*         This function produces delay only for the. <br>
*         If you don't need this functionality just leave it empty.<br>
*         n this case a compiler should optimize this function out.<br>
*
* \param[in] time_ms               time in milli seconds to delay
*/
void pal_os_event_delayms(uint32_t time_ms)
{

}

/**
* @}
*/
