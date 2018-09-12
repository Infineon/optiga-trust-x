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
* \brief   This file implements the platform abstraction layer APIs for socket communication.
*
* \ingroup  grPAL
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "DAVE.h"
#include "optiga/pal/pal_socket.h"
#include "optiga/pal/pal_os_timer.h"

#ifdef MODULE_ENABLE_DTLS_MUTUAL_AUTH

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
/// @cond hidden
#define PAL_SOCKET_TIMER_INTERVAL_5_MSEC (5000)

/**********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
typedef struct pal_socket_data_config {
    uint8_t *p_data;
    uint16_t p_length;
    uint8_t b_is_event_fired;
    ip_addr_t ip_address;
    int32_t i4RetVal;
} pal_socket_data_config, *p_pal_socket_data_config;

static uint32_t timer_lwip = 0;

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/
/// Close the socket communication and release the udp ports
void pal_socket_receive_handler(void * arg, struct udp_pcb * upcb, struct pbuf * p,
		                        ip_addr_t * addr, u16_t port);

//lint --e{715, 818} suppress "This is ignored as SYSTIMER_CALLBACK_t callback function prototype requires this argument"
void lwip_timer_isr(void *args)
{
    sys_check_timeouts();
}
/// @endcond

/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
/**
 * Assigns the IP address of the socket
 *
 * \param[in,out]  p_ip_address       Pointer to the location where the ip address tp be assigned
 * \param[in,out]  p_input_ip_address Pointer to the input IP address 
 *
 * \return  E_COMMS_SUCCESS on successful execution
 * \return  E_COMMS_FAILURE on failure
 */
//lint --e{714} suppress "Functions are extern and not reference in header file"
int32_t pal_socket_assign_ip_address(const char_t* p_ip_address,void* p_input_ip_address)
{
 
    int32_t i4RetVal = (int32_t) E_COMMS_FAILURE;

    ip_addr_t ip_address;
    if (0x01 == inet_aton(p_ip_address, &ip_address))
    {
        ((ip_addr_t *)p_input_ip_address)->addr = ip_address.addr;
        i4RetVal  = E_COMMS_SUCCESS;
    }
    
    return i4RetVal;
}

/**
 * Initializes socket communication structure
 *
 * \param[out]  p_socket Pointer to the socket communication structure
 *
 * \return  E_COMMS_SUCCESS on successful execution
 * \return  E_COMMS_FAILURE on failure
 * \return  E_COMMS_PARAMETER_NULL on parameter received is NULL
 * \return  E_COMMS_UDP_ALLOCATE_FAILURE on failure to allocate memory
 */
int32_t pal_socket_init(pal_socket_t* p_socket)
{
    int32_t i4RetVal = (int32_t) E_COMMS_FAILURE;

    do
    {
        //check for null values
        if (NULL == p_socket)
        {
            i4RetVal = (int32_t) E_COMMS_PARAMETER_NULL;
            break;
        }

        //create a new pcb structure
        //Initialise the structure using udp_new()
        p_socket->pcbTx = udp_new();
        if (NULL == p_socket->pcbTx)
        {
        	i4RetVal = (int32_t) E_COMMS_UDP_ALLOCATE_FAILURE;
            break;
        }

        p_socket->pfListen = pal_socket_receive_handler;
        
        if (timer_lwip == 0)
        {
            timer_lwip = SYSTIMER_CreateTimer(PAL_SOCKET_TIMER_INTERVAL_5_MSEC , SYSTIMER_MODE_PERIODIC, lwip_timer_isr, 0);
            //lint --e{534} suppress "Return value is not required to be checked"
            SYSTIMER_StartTimer(timer_lwip);
		}   

        i4RetVal = (int32_t) E_COMMS_SUCCESS;
    } while (FALSE);
    return i4RetVal;
}

/**
 * Opens an socket server port
 *
 * \param[out]  p_socket     Pointer to the socket communication structure
 * \param[in]   port        Port number for server
 *
 * \return  E_COMMS_SUCCESS on successful execution
 * \return  E_COMMS_PARAMETER_NULL on parameter received is NULL
 * \return  E_COMMS_UDP_BINDING_FAILURE on port binding failure
 * \return  E_COMMS_FAILURE on failure
 */
int32_t pal_socket_open(pal_socket_t* p_socket,
                        uint16_t port)
{
    int32_t i4RetVal = (int32_t) E_COMMS_FAILURE;

    do
    {
        //check for null values
        if (NULL == p_socket || NULL == p_socket->pcbTx)
        {
            i4RetVal = (int32_t) E_COMMS_PARAMETER_NULL;
            break;
        }

        i4RetVal = udp_bind(p_socket->pcbTx, &p_socket->sIPAddress, port);
        //bind to the port using udp_bind()
        if ((int32_t)ERR_OK != i4RetVal)
        {
        	i4RetVal = (int32_t) E_COMMS_UDP_BINDING_FAILURE;
            break;
        }

        p_socket->wPort = port;

        i4RetVal = (int32_t) E_COMMS_SUCCESS;
    } while (FALSE);
    return i4RetVal;
}

/**
 * Creates and connects to the client port
 *
 * \param[out]  p_socket     Pointer to the socket communication structure
 * \param[in]   port        Port number for server
 *
 * \return  E_COMMS_SUCCESS on successful execution
 * \return  E_COMMS_PARAMETER_NULL on parameter received is NULL
 * \return  E_COMMS_UDP_CONNECT_FAILURE on port binding failure
 * \return  E_COMMS_FAILURE on failure
 */
int32_t pal_socket_connect(pal_socket_t* p_socket,
                           uint16_t port)
{
    int32_t i4RetVal = (int32_t) E_COMMS_FAILURE;

    do
    {
        //check for null values
        if (NULL == p_socket || NULL == p_socket->pcbTx)
        {
            i4RetVal = (int32_t) E_COMMS_PARAMETER_NULL;
            break;
        }

        i4RetVal = udp_bind(p_socket->pcbTx, IP_ADDR_ANY, port);
        //bind to the port using udp_bind()
        if ((int32_t)ERR_OK != i4RetVal)
        {
        	i4RetVal = (int32_t) E_COMMS_UDP_CONNECT_FAILURE;
            break;
        }
        p_socket->wPort = port;

        i4RetVal = (int32_t) E_COMMS_SUCCESS;
    } while (FALSE);
    return i4RetVal;
}


/**
 * Transmits the data to the client from which the data was received.
 *
 * \param[in]  p_socket     Pointer to the socket communication structure
 * \param[in]  p_data       Pointer to the data buffer to be transmitted
 * \param[in]  length        The length of the data to be transmitted
 *
 * \return  E_COMMS_SUCCESS on successful execution
 * \return  E_COMMS_PARAMETER_NULL on parameter received is NULL
 * \return  E_COMMS_UDP_NO_DATA_TO_SEND on no data present to send
 * \return  E_COMMS_INSUFFICIENT_MEMORY on out of memory failure
 * \return  E_COMMS_UDP_ROUTING_FAILURE on failure to route the UDP packet
 * \return  E_COMMS_UDP_DEALLOCATION_FAILURE on failure to deallocate
 * \return  E_COMMS_FAILURE on failure
 */
int32_t pal_socket_send(const pal_socket_t* p_socket, uint8_t *p_data, uint32_t length)
{
    int32_t i4RetVal = (int32_t) E_COMMS_FAILURE;

    struct pbuf *p_out = NULL;
    struct pbuf *p_buffer_data = NULL;
	
    do
    {
        //check for null values
        if (NULL == p_socket || NULL == p_data
        		/*|| NULL == p_socket->pcbRx*/ || NULL == p_socket->pcbTx)
        {
            i4RetVal = (int32_t) E_COMMS_PARAMETER_NULL;
            break;
        }

        if (0 == length)
        {
        	i4RetVal = (int32_t) E_COMMS_UDP_NO_DATA_TO_SEND;
            break;
        }

        //allocate buffer using pbuf_alloc of length equal to PdwLen
        //As handler argument is expected the pcb that shall be used for
        //send back the data

        //Allocate a buffer to store data to be transmitted
        p_out = pbuf_alloc(PBUF_TRANSPORT, (uint16_t)length, PBUF_RAM);
        if (NULL == p_out)
        {
            i4RetVal = (int32_t) E_COMMS_INSUFFICIENT_MEMORY;
            break;
        }
        
        p_buffer_data = pbuf_alloc(PBUF_TRANSPORT, (uint16_t)0U, PBUF_RAM);
        if (NULL == p_buffer_data)
        {
            i4RetVal = (int32_t) E_COMMS_INSUFFICIENT_MEMORY;
            break;
        }
        p_buffer_data->payload = p_data;
        p_buffer_data->len = (uint16_t)length;
        p_buffer_data->next = NULL;
        p_buffer_data->tot_len = (uint16_t)length;
        
        //copy date to this buffer from Pp_data using pbuf_copy
        //lint --e{534} suppress "Return value is not required to be checked"
        pbuf_copy(p_out,p_buffer_data);

        //Send back data to same ip that sent the data on same port
        //send data to send port using udp_sendto
        i4RetVal = (int32_t) udp_sendto(p_socket->pcbTx, p_out,
                                &(p_socket->sIPAddress), p_socket->wPort);
        if ((int32_t)ERR_OK != i4RetVal)
        {
            //new error code for ERR_MEM?
        	if ((int32_t)ERR_MEM == i4RetVal)
        	{
        		i4RetVal = (int32_t) E_COMMS_INSUFFICIENT_MEMORY;
        	}
            //new error code for ERR_RTE?
        	else if((int32_t)ERR_RTE == i4RetVal)
        	{
        		i4RetVal = (int32_t) E_COMMS_UDP_ROUTING_FAILURE;
        	}
        	else
        	{
        		i4RetVal = (int32_t) E_COMMS_FAILURE;
        	}
            break;
        }

        i4RetVal = (int32_t) E_COMMS_SUCCESS;
    } while (FALSE);
		
	//clear allocated buffer pbuf_free
	if (i4RetVal != (int32_t) E_COMMS_INSUFFICIENT_MEMORY && 0 == pbuf_free(p_out))
	{
		i4RetVal = (int32_t) E_COMMS_UDP_DEALLOCATION_FAILURE;
	}
    
    if (i4RetVal != (int32_t) E_COMMS_INSUFFICIENT_MEMORY && 0 == pbuf_free(p_buffer_data))
	{
		i4RetVal = (int32_t) E_COMMS_UDP_DEALLOCATION_FAILURE;
	}

    return i4RetVal;
}


/**
 * Receives the data from the client
 *
 * \param[in,out]  p_socket     Pointer to the socket communication structure
 * \param[out]     p_data       Pointer to the data buffer to be received
 * \param[in,out]  p_length    Pointer to the length of the buffer
 *
 * \return  E_COMMS_SUCCESS on successful execution
 * \return  E_COMMS_PARAMETER_NULL on parameter received is NULL
 * \return  E_COMMS_UDP_NO_DATA_RECEIVED on no data received from the target
 * \return  E_COMMS_FAILURE on failure
 */
int32_t pal_socket_listen(pal_socket_t *p_socket,
                          uint8_t *p_data, uint32_t *p_length)
{
    int32_t i4RetVal = (int32_t) E_COMMS_FAILURE;

    pal_socket_data_config sInOutDataToCallBack = { 0 };
    uint32_t wTickCount = 0;
    uint32_t wBaseTickCount = 0;

    do
    {
        //check for null values
        if (NULL == p_socket || NULL == p_data || NULL == p_length || NULL == p_socket->pcbTx)
        {
            i4RetVal = (int32_t) E_COMMS_PARAMETER_NULL;
            break;
        }
        //Register an handler on receive packet event using udp_recv()
        sInOutDataToCallBack.p_length = (uint16_t)*p_length;
        sInOutDataToCallBack.p_data = p_data;
        sInOutDataToCallBack.b_is_event_fired = FALSE;

        wBaseTickCount = pal_os_timer_get_time_in_milliseconds();
        //enable receive event
        udp_recv(p_socket->pcbTx, (udp_recv_fn)p_socket->pfListen,
                &sInOutDataToCallBack);


        if ((uint8_t)eNonBlock == p_socket->bMode)
        {
            do
            {  
                //blocking receive call: wait till receive event fires              
                wTickCount = pal_os_timer_get_time_in_milliseconds();   
            }while (((wTickCount-wBaseTickCount) <= (uint32_t)(p_socket->wTimeout))&&
            (sInOutDataToCallBack.b_is_event_fired == FALSE));
        }
        else
        {
            do
            {
                //blocking receive call: wait till receive event fires
                if (sInOutDataToCallBack.b_is_event_fired == TRUE)
                    break;           
            }while (1);
        } 
       
        //Exit after timeout
        if (sInOutDataToCallBack.b_is_event_fired != TRUE)
        {
            i4RetVal = (int32_t) E_COMMS_UDP_NO_DATA_RECEIVED;
            break;
        }

        if (E_COMMS_SUCCESS != sInOutDataToCallBack.i4RetVal)
        {
            i4RetVal = sInOutDataToCallBack.i4RetVal;
            break;
        }

        if (0 == sInOutDataToCallBack.p_length)
        {
            i4RetVal = (int32_t) E_COMMS_UDP_NO_DATA_RECEIVED;
            break;
        }

        p_data = sInOutDataToCallBack.p_data;
        *p_length = sInOutDataToCallBack.p_length;
        p_socket->sIPAddress = sInOutDataToCallBack.ip_address;

        i4RetVal = (int32_t) E_COMMS_SUCCESS;
    } while (FALSE);
		
    if ((NULL != p_socket) && (NULL != p_socket->pcbTx))
    {
        //disable receiving of data till next receive call
        udp_recv(p_socket->pcbTx, NULL, NULL);
    }

    return i4RetVal;
}

/**
 * Closes the UDP communication and releases all the resources
 *
 * \param[in]  p_socket     Pointer to the socket communication structure
 *
 * \return  None
 */
void pal_socket_close(pal_socket_t* p_socket)
{
    //check for null values
    if (NULL != p_socket)
    {
        if (NULL != p_socket->pcbTx)
        {
            udp_disconnect(p_socket->pcbTx);
            udp_remove(p_socket->pcbTx);
        }
		
        p_socket->pfListen = NULL;

        memset(&p_socket->sIPAddress, 0, sizeof(p_socket->sIPAddress));
        p_socket->wPort = 0;
	}
}
/// @cond hidden
/** 
 * Receive event handler function.
 *
 * \param[in,out]   arg      Pointer to the buffer to be received, IP address of the client and buffer length
 * \param[in]       upcb     The udp_pcb which received data
 * \param[in]       p_pbuf   Pointer to the packet buffer to be received
 * \param[in]       addr     The remote IP address from which the packet was received
 * \param[in]       port     The remote port from which the packet was received
 *
 * \return  None
 */
//lint --e{715,830,818} suppress  " Interrupt Handler for recieve"
void pal_socket_receive_handler(void * arg, struct udp_pcb * upcb, struct pbuf * p_pbuf,
		                        ip_addr_t * p_addr, u16_t port)
{
    struct pbuf * q;
    uint32_t dwOffset = 0;
    
    if(NULL != arg)
    {
        ((p_pal_socket_data_config) arg)->i4RetVal = (int32_t) E_COMMS_FAILURE;
            do
            {
                if (NULL == p_pbuf || NULL == p_pbuf->payload || NULL == p_addr
                        || NULL == ((p_pal_socket_data_config) arg)->p_data)
                {
                    ((p_pal_socket_data_config) arg)->p_length = 0;
                    ((p_pal_socket_data_config) arg)->i4RetVal = (int32_t) E_COMMS_PARAMETER_NULL;
                    break;
                }
                else if (((p_pal_socket_data_config) arg)->p_length < p_pbuf->tot_len)
                {
                    ((p_pal_socket_data_config) arg)->p_length = 0;
                    ((p_pal_socket_data_config) arg)->i4RetVal = (int32_t) E_COMMS_INSUFFICIENT_BUF_SIZE;
                    break;
                }
                
                ((p_pal_socket_data_config) arg)->p_length = p_pbuf->tot_len;
                ((p_pal_socket_data_config) arg)->ip_address = *p_addr;
                
                for (q = p_pbuf; q != NULL; q = q->next)
                {
                    memcpy( ((((p_pal_socket_data_config) arg)->p_data)+dwOffset), q->payload, q->len);
                    dwOffset += q->len;
                }

                ((p_pal_socket_data_config) arg)->i4RetVal = (int32_t) E_COMMS_SUCCESS;
                
            } while (FALSE);
            //Free allocated buffer of received data
            ((p_pal_socket_data_config) arg)->b_is_event_fired = TRUE;
    }
    //lint --e{534} suppress "This is a void function so return value is ignored"
    pbuf_free(p_pbuf);
    return;
}

#endif
/// @endcond
/**
* @}
*/
