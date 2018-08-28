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
* \file example_optiga_crypt_tls_prf_sha256.c
*
* \brief   This file provides the example for key derivation (PRF SHA256 as per TLS v1.2) using 
*          #optiga_crypt_tls_prf_sha256.
*
* \ingroup
* @{
*/

#include "optiga/optiga_crypt.h"
#include "optiga/optiga_util.h"

/**
 * The below example demonstrates the key derivation using #optiga_crypt_tls_prf_sha256.
 *
 */
optiga_lib_status_t example_optiga_crypt_tls_prf_sha256(void)
{
    uint8_t label [] = "Firmware update";
    uint8_t coprocessor_uid[27];
    uint16_t coprocessor_uid_len = sizeof(coprocessor_uid);

    uint8_t random_seed [] = {
        0x61, 0xC7, 0xDE, 0xF9, 0x0F, 0xD5, 0xCD, 0x7A, 
        0x8B, 0x7A, 0x36, 0x41, 0x04, 0xE0, 0x0D, 0x82, 
        0x38, 0x46, 0xBF, 0xB7, 0x70, 0xEE, 0xBF, 0x8F, 
        0x40, 0x25, 0x2E, 0x0A, 0x21, 0x42, 0xAF, 0x9C,
    };

    // Secret to be written to data object which will be
    // later used as part of TLS PRF SHA256 key derivation
    uint8_t secret_to_be_written [] = {
        0xBF, 0xB7, 0x70, 0xEE, 0xBF, 0x8F, 0x61, 0xC7, 
        0x04, 0xE0, 0x0D, 0x82, 0x8B, 0x7A, 0x36, 0x41,  
        0xD5, 0xCD, 0x7A, 0x38, 0x46, 0xDE, 0xF9, 0x0F,  
        0x21, 0x42, 0x40, 0x25, 0x0A, 0xAF, 0x9C, 0x2E, 
    };

    uint8_t decryption_key [16] = {0};

    optiga_lib_status_t return_status;

    do
    {

        /* 0. FW version before 1.40.1118 doesn't support derive key
         *    function from an arbitrary OID, only from session OID
        */
        return_status = optiga_util_read_data(eCOPROCESSOR_UID,
        									  0x00,
											  coprocessor_uid,
											  &coprocessor_uid_len);

        if ((coprocessor_uid[26] < 11) || (coprocessor_uid[27] < 18))
        {
        	/* This example can't work with this firmware, you need
        	 * to populate and use session OID instead
        	 */
        	return_status = OPTIGA_LIB_SUCCESS;
        	break;
        }


        /* 1. Write the shared secret to the Arbitrary data object F1D0
        *       - This is typically a one time activity and
        *       - use the this OID as input secret to derive keys further
        */
        return_status = optiga_util_write_data(0xF1D0,
        									   OPTIGA_UTIL_ERASE_AND_WRITE,
                                               0x00,
                                               secret_to_be_written,
                                               sizeof(secret_to_be_written));

        if(OPTIGA_LIB_SUCCESS != return_status)
        {
        	//Write to data object is failed.
            break;
        }

        /**
         * Derive key (e.g. decryption key) using optiga_crypt_tls_prf_sha256 with I2C communication.
         *       - Use shared secret from F1D0 data object
         */

        return_status = optiga_crypt_tls_prf_sha256(0xF1D0, /* Input secret OID */
                                                    label,
                                                    sizeof(label),
                                                    random_seed,
                                                    sizeof(random_seed),
                                                    sizeof(decryption_key),
                                                    TRUE,
                                                    decryption_key);

        if(OPTIGA_LIB_SUCCESS != return_status)
        {
			//Derive key operation failed.
            break;
        }

    } while(FALSE);

    return return_status;
}
/**
* @}
*/
