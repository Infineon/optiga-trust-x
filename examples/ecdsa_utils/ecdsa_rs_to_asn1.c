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

optiga_lib_status_t ecdsa_rs_to_asn1(uint8_t const * p_r,
                      size_t          r_len,
                      uint8_t const * p_s,
                      size_t          s_len,
                      uint8_t       * p_asn_sig,
                      size_t        * p_asn_sig_len)
{
    uint32_t index = 0;
    optiga_lib_status_t return_status = OPTIGA_LIB_ERROR;
    uint8_t r_pad = 0;
    uint8_t s_pad = 0;
    uint8_t* _p_r = (uint8_t  *)p_r;
    uint8_t* _p_s = (uint8_t  *)p_s;
    int i = 0;

    do {
		if (_p_r == NULL || _p_s == NULL || p_asn_sig_len == NULL)
		{
			break;
		}

		if (r_len == 0 || r_len > DER_INTEGER_MAX_LEN || s_len == 0 || s_len > DER_INTEGER_MAX_LEN)
		{
			break;
		}

		if(*p_asn_sig_len < (r_len + s_len + DER_OVERHEAD)) {
			// not enough space in output buffer
			break;
		}

		// R component
		p_asn_sig[0] = 0x02;
		i = 0;
		while(*(_p_r + r_pad) == 0x00)
		{
			r_pad++;
		}
		p_asn_sig[1] = 0x20 - r_pad;

		if (_p_r[r_pad] & 0x80)
		{
			p_asn_sig[1] += 1;
			p_asn_sig[2] =  0;
			index++;
		}
		_p_r += r_pad;
		r_len -= r_pad;
		memcpy(p_asn_sig + index + 2, _p_r, r_len);

		index += r_len + 2;
		// S component
		p_asn_sig[index + 0] = 0x02;
		p_asn_sig[index + 1] = 0x20;

		while(*(p_s+s_pad) == 0x00)
		{
			s_pad++;
		}
		p_asn_sig[index + 1] = 0x20 - s_pad;

		if (p_s[s_pad] & 0x80)
		{
			p_asn_sig[index + 1] += 1;
			p_asn_sig[index + 2] =  0;
			index++;
		}
		_p_s += s_pad;
		s_len -= s_pad;

		memcpy(p_asn_sig + index + 2, _p_s, s_len);
		index += s_len + 2;

		*p_asn_sig_len = index; // Return total length of ASN.1-encoded data structure
    } while(0);

    return return_status;
}

/**
* @}
*/
