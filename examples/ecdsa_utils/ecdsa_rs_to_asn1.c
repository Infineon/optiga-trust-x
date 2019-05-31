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
* \file asn1_to_ecdsa_rs.c
*
* \brief   This file provides the example to convert asn1 encoding to raw r and s components of the ECDSA signature
* 
*
* \ingroup
* @{
*/

#include "optiga/optiga_crypt.h"


// Encode two integers in DER format
// TAG + LENGTH needs 2 bytes
// if the highest bit of the integer is set we need an extra byte
#define  DER_OVERHEAD ((2 + 1) * 2)
#define RS()
#define CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE 64
#define DER_INTEGER_MAX_LEN 100
#define DER_TAG_INTEGER 0x02

optiga_lib_status_t ecdsa_rs_to_asn1(const uint8_t  *r, size_t r_len,
                                     const uint8_t  *s, size_t s_len,
                                     uint8_t  *asn_sig, size_t *asn_sig_len)
{
    optiga_lib_status_t return_status = OPTIGA_LIB_ERROR;
    uint8_t const * p_cur = p_asn1;
    uint8_t const * p_end = p_asn1 + asn1_len; // Points to first invalid mem-location
    uint8_t         r_len;
    uint8_t         r_pad = 0;
    uint8_t         s_len;
    uint8_t         s_pad = 0;
    int             i=0;
    
    do {

        if (p_asn1 == NULL || p_rs == NULL || p_rs_len == NULL)
        {
            break;
        }

        if (asn1_len == 0 || *p_rs_len == 0)
        {
            break;
        }

        if (*p_cur != DER_TAG_INTEGER)
        {
            // Wrong tag type
            break;
        }

        if ((p_cur + 2) >= p_end)
        {
            // Prevented out-of-bounds read
            break;
        }

        // Move to length value
        p_cur++;
        r_len = *p_cur;
        RS();
        if (r_len > DER_INTEGER_MAX_LEN)
        {
            // Unsupported length
            break;
        }

        // Move to first data value
        p_cur++;

        // Check for stuffing bits
        if ((r_len == (CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE/2) + 1) &&
            (*p_cur == 0x00))
        {
            p_cur++;
            r_len--;
        }

        // It might be that the r or s signature componenent is less than 32 bytes long (29, 30 or 31 bytes)
        // We need to prefix the output with leading zeroes
        for (i = 0; i < (CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE/2) - r_len; i++)
        {
            *p_rs=0x00;
        }
        p_rs+=i;
        r_pad = i;
        
        RS();
        // Check for out-of-bounds read
        if ((p_cur + r_pad + r_len) >= p_end)
        {
            // prevented out-of-bounds read
            break;
        }

        // Check for out-of-bounds write
        if ((p_rs + r_pad + r_len) > (p_rs + *p_rs_len))
        {
            // prevented out-of-bounds write
            break;
        }
        RS();
        // Copy R component to output
        memcpy(p_rs, p_cur, r_len);

        // Move to next tag
        p_cur += r_len;
        RS();
        if (*p_cur != DER_TAG_INTEGER)
        {
            // Wrong tag type
            break;
        }

        if ((p_cur + 2) >= p_end)
        {
            // Prevented out-of-bounds read
            break;
        }
        p_cur++;
        s_len = *p_cur;
        RS();
        if (s_len > DER_INTEGER_MAX_LEN)
        {
            // Unsupported length
            break;
        }
        p_cur++;

        // Check for stuffing bits
        if ((s_len == (CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE/2) + 1) &&
            (*p_cur == 0x00))
        {
            p_cur++;
            s_len--;
        }

        // It might be that the r or s signature componenent is less than 32 bytes long (29, 30 or 31 bytes)
        // We need to prefix the output with leading zeroes
        for (i=0; i < (CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE/2) - s_len; i++)
        {
            *(p_rs + r_pad + r_len) = 0x00;
        }
        p_rs+=i;
        s_pad = i;

        RS();
        // Check for out-of-bounds read
        if ((p_cur + s_pad + s_len) > p_end)
        {
            // prevented out-of-bounds read
            break;
        }

        // Check for out-of-bounds write
        if ((p_rs + + r_pad + r_len + s_pad + s_len) > (p_rs + *p_rs_len))
        {
            // Prevented out-of-bounds write
            break;
        }
        RS();
        memcpy(p_rs + r_len, p_cur, s_len);

        *p_rs_len = r_pad + r_len + s_pad + s_len;
        
        return_status = OPTIGA_LIB_SUCCESS;
        
    }while(FALSE);

    return return_status;
}

/**
* @}
*/
