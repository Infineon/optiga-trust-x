/**
* MIT License
*
* Copyright (c) 2019 Arrow Electronics
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
* \file pal_os_lock.c
*
* \brief   This file implements the platform abstraction layer APIs for os locks (e.g. semaphore).
*
* \ingroup  grPAL
* @{
*/
/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
/* Optiga based includes */
#include <optiga/pal/pal_os_lock.h>

/* Zephyr based includes */
#include <zephyr.h>

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define SEM_INIT_VALUE      1
#define SEM_MAX_VALUE       1
#define SEM_TAKE_SUCCESS    0

/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
/* binary semaphore for managing concurrent crypto calls */
K_SEM_DEFINE(pal_lock, SEM_INIT_VALUE, SEM_MAX_VALUE);

/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
pal_status_t pal_os_lock_acquire(void)
{
    pal_status_t return_status = PAL_STATUS_FAILURE;

    if (k_sem_take(&pal_lock, K_FOREVER) == SEM_TAKE_SUCCESS) {
        return_status = PAL_STATUS_SUCCESS;
    }

    return return_status;
}

void pal_os_lock_release(void)
{
    k_sem_give(&pal_lock);
}

/**
* @}
*/
