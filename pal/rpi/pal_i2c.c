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
* \brief This file implements the platform abstraction layer APIs for I2C interface.
*
* \ingroup  grPAL
* @{
*/

#include "optiga/pal/pal_i2c.h"
#include "optiga/ifx_i2c/ifx_i2c_config.h" // IFX_I2C_LOG_PAL
#include "optiga/ifx_i2c/ifx_i2c.h"

#include <stdio.h>  // printf()
#include <unistd.h> // read(), write()
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>


#if IFX_I2C_LOG_PAL == 1
#include <stdio.h>
#define LOG(...)  printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define LOG_PREFIX "[IFX-PAL-I2C] "

#define I2C_SLAVE       (0x703)            /* defined in <linux/i2c-dev.h> */
#define I2C_DEVICE      "/dev/i2c-1"


static void invoke_upper_layer_callback(const pal_i2c_t* p_pal_i2c_ctx, host_lib_status_t event);


/* Pointer to the I2C file */
static int i2c_file;


static void invoke_upper_layer_callback(const pal_i2c_t* p_pal_i2c_ctx, host_lib_status_t event)
{
    app_event_handler_t upper_layer_handler = (app_event_handler_t) p_pal_i2c_ctx->upper_layer_event_handler;
    upper_layer_handler(p_pal_i2c_ctx->upper_layer_ctx, event);
}


pal_status_t pal_i2c_init(const pal_i2c_t* p_i2c_context)
{
	LOG(LOG_PREFIX "pal_i2c_init(...): >\n");

    pal_status_t pal_status = PAL_STATUS_FAILURE;
    int32_t      i2c_status;

    do
    {
        if ((i2c_file = open(I2C_DEVICE, O_RDWR)) < 0)
        {
        	LOG(LOG_PREFIX "pal_i2c_init(...): failed to open i2c_file, open(...)=0x%x\n", i2c_file);
            pal_status = PAL_STATUS_FAILURE;
            break;
        }
  
        i2c_status = ioctl(i2c_file, I2C_SLAVE, p_i2c_context->slave_address);
        if (0 != i2c_status)
        {
        	LOG(LOG_PREFIX "pal_i2c_init(...): failed to ioctl i2c_file, ioctl(...)=0x%x\n", i2c_status);
            pal_status = PAL_STATUS_FAILURE;
            close(i2c_file);
            break;
        }

        pal_status = PAL_STATUS_SUCCESS;
    } while (0);

    LOG(LOG_PREFIX "pal_i2c_init(...): < with pal_status=0x%x\n", pal_status);

    return pal_status;
}


pal_status_t pal_i2c_deinit(const pal_i2c_t* p_i2c_context)
{
    LOG(LOG_PREFIX "pal_i2c_deinit(...): >\n");

    close(i2c_file);
    return PAL_STATUS_SUCCESS;
}


pal_status_t pal_i2c_write(pal_i2c_t* p_i2c_context, uint8_t* p_data , uint16_t length)
{
	LOG(LOG_PREFIX "pal_i2c_write(...): >\n");

    pal_status_t pal_status = PAL_STATUS_FAILURE;
    int32_t      i2c_status;

    i2c_status = write(i2c_file, p_data, length);
    if (length == i2c_status)
    {
    	invoke_upper_layer_callback(p_i2c_context, PAL_I2C_EVENT_SUCCESS);
        pal_status = PAL_STATUS_SUCCESS;
    }
    else
    {
    	LOG(LOG_PREFIX "pal_i2c_write(...): failed to write(i2c_file=0x%x, p_data=0x%x, length=0x%x)=0x%x\n",
    			i2c_file, (uint32_t) p_data, length, i2c_status);
    	invoke_upper_layer_callback(p_i2c_context, PAL_I2C_EVENT_ERROR);
        pal_status = PAL_STATUS_FAILURE;
    }

    return pal_status;
}

pal_status_t pal_i2c_read(pal_i2c_t* p_i2c_context, uint8_t* p_data, uint16_t length)
{
	LOG(LOG_PREFIX "pal_i2c_read(...): >\n");

    pal_status_t pal_status = PAL_STATUS_FAILURE;
    int32_t      i2c_status;

    i2c_status = read(i2c_file, p_data, length);
    if (i2c_status == length)
    {
    	invoke_upper_layer_callback(p_i2c_context, PAL_I2C_EVENT_SUCCESS);
        pal_status = PAL_STATUS_SUCCESS;
    }
    else
    {
    	invoke_upper_layer_callback(p_i2c_context, PAL_I2C_EVENT_ERROR);
        pal_status = PAL_STATUS_FAILURE;
    }
        
    return pal_status;
}

pal_status_t pal_i2c_set_bitrate(const pal_i2c_t* p_i2c_context , uint16_t bitrate)
{
	LOG(LOG_PREFIX "pal_i2c_set_bitrate(...): >\n");

    // Bitrate is fixed to the maximum frequency on this platform (400K)
    return PAL_STATUS_SUCCESS;
}
