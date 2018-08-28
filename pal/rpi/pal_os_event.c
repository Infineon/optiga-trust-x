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
* \brief This file implements the platform abstraction layer APIs for event management.
*
* \ingroup  grPAL
* @{
*/

#include "optiga/ifx_i2c/ifx_i2c_config.h" // IFX_I2C_LOG_PAL
#include "optiga/pal/pal_os_event.h"
#include "optiga/pal/pal_os_timer.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h> // usleep()


#define ERR(...)  fprintf(stderr, __VA_ARGS__)


#if IFX_I2C_LOG_PAL == 1
#include <stdio.h>
#define LOG(...)  printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define LOG_PREFIX "[IFX-PAL-OS-EVENT] "


typedef struct callback_context
{
    register_callback  function;
	void*              arguments;
	uint32_t           timer_delay_ms;

} callback_context_t;


static void* timer_thread(void* p_arg);
static void  invoke_timer_callback(callback_context_t* context);


#ifdef DEBUG
static uint32_t total_thread_count = 0;
static uint32_t activ_thread_count = 0;
#endif

void pal_os_event_init()
{
    LOG(LOG_PREFIX "pal_os_event_init() ><\n");
}


void pal_os_event_register_callback_oneshot(register_callback callback, 
                                            void*             callback_args,
                                            uint32_t          time_us)
{
	LOG(LOG_PREFIX "pal_os_event_register_callback_oneshot() >\n");

	int32_t             result;
	pthread_t           timer_thread_handle;
	callback_context_t* context             = NULL;

	context = malloc(sizeof(callback_context_t));
	if (NULL == context)
	{
		ERR(LOG_PREFIX "pal_os_event_register_callback_oneshot() ERROR - out of memory (malloc failed)\n");
		exit(-1); // Hard exit
	}
	context->function       = callback;
	context->arguments      = callback_args;
	context->timer_delay_ms = (time_us / 1000) + 1;

	result = pthread_create(&timer_thread_handle, NULL, timer_thread, context);
	// On success, pthread_create() returns 0; on error, it returns an error number
	if (0 != result)
	{
		ERR(LOG_PREFIX "pal_os_event_register_callback_oneshot() ERROR on thread creation: %d=0x%x\n", result, result);
#ifdef DEBUG
		ERR(LOG_PREFIX "pal_os_event_register_callback_oneshot() ERROR {total,activ}_thread_count={%d,%d}\n",
			total_thread_count, activ_thread_count);
#endif
		// Variant 1: hard exit
		//exit(-1);
        // Variant 2: Synchronously call the handler
		// HBackup: sleep for requested callback time, then invoke callback
		pal_os_timer_delay_in_milliseconds(context->timer_delay_ms);
		invoke_timer_callback(context);

	}
#ifdef DEBUG
	total_thread_count++;
	activ_thread_count++;
#endif

    LOG(LOG_PREFIX "pal_os_event_register_callback_oneshot() <\n");
}


static void invoke_timer_callback(callback_context_t* context)
{
	LOG(LOG_PREFIX "timer_callback() >\n");

    if ((NULL != context) && (NULL != context->function))
    {
        context->function(((void*)(context->arguments)));
    }
    else
    {
    	ERR(LOG_PREFIX "invoke_timer_callback() ERROR: context for callback function is NULL; invoking nothing\n");
    	exit(-1);
    }

	LOG(LOG_PREFIX "timer_callback() <\n");
}


static void* timer_thread(void* p_arg)
{
	LOG(LOG_PREFIX "timer_thread() >\n");

	callback_context_t* p_context;
	uint32_t            time_elapsed_ms;
	uint32_t            start_tick_ms;

	p_context = (callback_context_t*) p_arg;

	LOG(LOG_PREFIX "timer_thread() context.timer_delay_ms=%d\n", p_context->timer_delay_ms);
	LOG(LOG_PREFIX "timer_thread() context.function=%x\n", (uint32_t) p_context->function);
	LOG(LOG_PREFIX "timer_thread() context.arguments=%x\n", (uint32_t) p_context->arguments);

	time_elapsed_ms = 0;
	start_tick_ms   = pal_os_timer_get_time_in_milliseconds();

	while (time_elapsed_ms < p_context->timer_delay_ms)
	{
		time_elapsed_ms = pal_os_timer_get_time_in_milliseconds() - start_tick_ms;
	}
	LOG(LOG_PREFIX "timer_thread() time_elapsed_ms=%d < p_context->timer_delay_ms=%d\n", time_elapsed_ms, p_context->timer_delay_ms);

	invoke_timer_callback(p_context);

	LOG(LOG_PREFIX "timer_thread() < pthread_exit(NULL)\n");
	free(p_context);
#ifdef DEBUG
	activ_thread_count--;
#endif
	pthread_exit(NULL);
}
