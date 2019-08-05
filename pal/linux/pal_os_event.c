/**
* \copyright
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
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file pal_os_event.c
*
* \brief   This file implements the platform abstraction layer APIs for os event/scheduler.
*
* \ingroup  grPAL
* @{
*/

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "optiga/pal/pal_os_timer.h"
#include "optiga/pal/pal_os_event.h"

#include "pal_linux.h"


#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

/** \brief PAL os event structure */
typedef struct pal_os_event
{
    /// registered callback
    register_callback callback_registered;
    /// context to be passed to callback
    void * callback_ctx;
}pal_os_event_t;

static pal_os_event_t pal_os_event_0 = {0};
static 	timer_t timerid;

static void handler(int sig, siginfo_t *si, void *uc)
{
	register_callback callback;
	
	if (pal_os_event_0.callback_registered)
    {
        callback = pal_os_event_0.callback_registered;
        pal_os_event_0.callback_registered = NULL;
        callback((void * )pal_os_event_0.callback_ctx);
    }
}

pal_status_t pal_os_event_init(void)
{
	struct sigevent sev;
	struct sigaction sa;
	
	/* Establishing handler for signal */
	
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
	{
		printf("sigaction\n");
		exit(1);
	}

	/* Create the timer */

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCKID, &sev, &timerid) == -1)
	{
		printf("timer_create\n");
		exit(1);
	}
	
	return PAL_STATUS_SUCCESS;
}


void pal_os_event_register_callback_oneshot(register_callback callback, 
                                            void*             callback_args,
                                            uint32_t          time_us)
{
	struct itimerspec its;
	long long freq_nanosecs;
    pal_os_event_0.callback_registered = callback;
    pal_os_event_0.callback_ctx = callback_args;
	
	/* Start the timer */

	freq_nanosecs = time_us * 1000;
	its.it_value.tv_sec = freq_nanosecs / 1000000000;
	its.it_value.tv_nsec = freq_nanosecs % 1000000000;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 0;
	
	if (timer_settime(timerid, 0, &its, NULL) == -1)
	{
		printf("Error in timer_settime\n");
	    exit(1);
	}
}

/**
* @}
*/

