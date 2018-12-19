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
* \file pal.c
*
* \brief    This file implements the platform abstraction layer APIs.
*
* \ingroup  grPAL
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
//#include <DAVE.h>
#include "optiga/pal/pal_i2c.h"
#ifdef __WIN32__
#include "libusb.h"
#else // LINUX
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#endif
#include "pal_usb.h"
#include "pal_common.h"

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
// Setup debug log statements
#if IFX_I2C_LOG_HAL == 1
#define LOG_PAL IFX_I2C_LOG
#else
#include<stdio.h>
#define LOG_PAL(...) //printf(__VA_ARGS__)
#endif

#define WAIT_500_MS    (500)
/// @cond hidden
/*********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/

extern pal_usb_t usb_events;
static libusb_device_handle *dev_handle = NULL; //a device handle

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/

pal_status_t pal_init(void)
{

    struct libusb_device_descriptor dev_desc;
	struct libusb_config_descriptor* config_desc = NULL;
	ssize_t number_of_connected_devices; //holding number of devices in list
	ssize_t k; //for iterating through the list
	int ftdi_dev_num = 0;
	int ftdi_dev;
	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	unsigned char strDesc[256];
	
	if (libusb_init(NULL))
	{
		LOG_PAL("Failed to init libusb\n.");
		return PAL_I2C_EVENT_ERROR;
	}
	LOG_PAL("pal_init\n. ");

	//libusb_set_debug(NULL, 4);
/*	
	number_of_connected_devices = libusb_get_device_list(NULL, &devs); //get the list of devices
	
	if (number_of_connected_devices < 0)
	{
	 LOG_PAL("No devices found\n.");
	 return PAL_I2C_EVENT_ERROR;
	}
	
	for(k=0;k<number_of_connected_devices;k++)
	{
	 int a = libusb_get_device_descriptor(devs[k], &dev_desc);
	 if (a < 0)
	 {
		 LOG_PAL("Failed to get usb descriptor\n.");
		 return PAL_I2C_EVENT_ERROR;
	 }
	 else
	 {
		 LOG_PAL("Found [%X:%X]\n.", dev_desc.idVendor, dev_desc.idProduct);
	 }
	
	 if ( (dev_desc.idVendor == USB_VID) && (dev_desc.idProduct == USB_PID))
	 {
		 ftdi_dev_num++;
		 ftdi_dev = k;
		 break;
	 }
	}

	if (ftdi_dev_num > 1)
	{
	 LOG_PAL("Error: more than one device connected! %d\n.", ftdi_dev_num);
	 return PAL_I2C_EVENT_ERROR;
	}

	if (libusb_open(devs[ftdi_dev], &dev_handle) < 0)
	{
	 LOG_PAL("Error: cannot open device!\n.");
	 return PAL_I2C_EVENT_ERROR;
	}
	
	libusb_get_string_descriptor_ascii(dev_handle, dev_desc.iManufacturer, strDesc, 256);
	LOG_PAL("Manufacturer: %s\n", strDesc);
	libusb_get_string_descriptor_ascii(dev_handle, dev_desc.iSerialNumber, strDesc, 256);
	LOG_PAL("SerialNumber: %s\n", strDesc);
	libusb_get_string_descriptor_ascii(dev_handle, dev_desc.iProduct, strDesc, 256);
	LOG_PAL("Product: %s\n", strDesc);
	
	if (libusb_claim_interface(dev_handle, 0) < 0)
	{
		libusb_close(dev_handle);
		dev_handle=NULL;
		return PAL_I2C_EVENT_ERROR;
	}
	
	libusb_free_device_list(devs, number_of_connected_devices);
*/
	dev_handle = libusb_open_device_with_vid_pid(NULL, USB_VID, USB_PID);
	
	usb_events.handle = dev_handle;

	if (usb_events.handle == NULL)
	{
		LOG_PAL("Error: dev_handle is NULL!\n.");
		return PAL_I2C_EVENT_ERROR;
	}

#ifndef __WIN32__ // LINUX
    libusb_detach_kernel_driver(usb_events.handle, USB_INTERFACE);
#endif

	if (libusb_get_active_config_descriptor(libusb_get_device(usb_events.handle), &config_desc))
	{
		return PAL_I2C_EVENT_ERROR;
	}

	if (config_desc->bNumInterfaces < 1 || config_desc->interface->num_altsetting < 1
		|| config_desc->interface->altsetting->bNumEndpoints < 2)
	{
		return PAL_I2C_EVENT_ERROR;
	}

	usb_events.hid_ep_in = config_desc->interface->altsetting->endpoint->bEndpointAddress;
	if (config_desc->interface->altsetting->endpoint->wMaxPacketSize != HID_REPORT_SIZE)
	{
		return PAL_I2C_EVENT_ERROR;
	}

	usb_events.hid_ep_out = (config_desc->interface->altsetting->endpoint + 1)->bEndpointAddress;
	if (((config_desc->interface->altsetting->endpoint) + 1)->wMaxPacketSize != HID_REPORT_SIZE)
	{
		return PAL_I2C_EVENT_ERROR;
	}

	if (ifx_i2c_usb_reset(usb_events) != PAL_STATUS_SUCCESS)
	{
		return PAL_I2C_EVENT_ERROR;
	}
	return PAL_STATUS_SUCCESS;
}

pal_status_t pal_deinit(void)
{
    return PAL_STATUS_SUCCESS;
}
