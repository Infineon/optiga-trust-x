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
*/

#include "sdk_config.h"
#include "nordic_common.h"

#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "nrf_crypto_ecc.h"
#include "nrf_crypto_ecdsa.h"

/*lint -save -e????*/
#include "optiga/optiga_crypt.h"
/*lint -restore*/

/**
 * @brief decodes two ASN.1 integers to the R and S components of a ECC signature
 * @param[in] asn1 buffer containing the ASN.1 encoded R and S values
 * @param[in] asn1_len length of the asn1 buffer
 * @param[out] rs buffer where to write the R and S values
 * @param[in,out] rs_len length of the rs buffer, contains the bytes written afterwards
 * @returns NRF_SUCCESS on success, NRF_ERROR_CRYPTO_INTERNAL else
 */
static ret_code_t asn1_to_ecdsa_rs(const uint8_t * asn1, size_t asn1_len,
                                   uint8_t * rs, size_t * rs_len)
{
    if(asn1 == NULL || rs == NULL || rs_len == NULL) {
        return NRF_ERROR_CRYPTO_INTERNAL;
    }

    if(asn1_len == 0 || *rs_len == 0) {
        return NRF_ERROR_CRYPTO_INTERNAL;
    }

    const uint8_t * cur = asn1;
    // points to first invalid mem-location
    const uint8_t * end = asn1 + asn1_len;
    if(*cur != 0x02) {
        // wrong tag type
        return NRF_ERROR_CRYPTO_INTERNAL;
    }

    if((cur + 2) >= end) {
        // prevented out-of-bounds read
        return NRF_ERROR_CRYPTO_INTERNAL;
    }
    
    // move to length value
    cur++;
    uint8_t r_len = *cur;

    // move to first data value
    cur++;

    // check for stuffing bits
    if(*cur == 0x00) {
        cur++;
        r_len--;
    }

    // check for out-of-bounds read
    if((cur + r_len) >= end) {
        // prevented out-of-bounds read
        return NRF_ERROR_CRYPTO_INTERNAL;
    }
    // check for out-of-bounds write
    if((rs + r_len) > (rs + *rs_len)) {
        // prevented out-of-bounds write
        return NRF_ERROR_CRYPTO_INTERNAL;
    }
    
    // copy R component to output
    memcpy(rs, cur, r_len);

    // move to next tag
    cur += r_len;

    if(*cur != 0x02) {
        // wrong tag type
        return NRF_ERROR_CRYPTO_INTERNAL;
    }

    if((cur + 2) >= end) {
        // prevented out-of-bounds read
        return NRF_ERROR_CRYPTO_INTERNAL;
    }
    cur++;
    uint8_t s_len = *cur;
    cur++;

    if(*cur == 0x00) {
        cur++;
        s_len--;
    }

    // check for out-of-bounds read
    if((cur + s_len) > end) {
        // prevented out-of-bounds read
        return NRF_ERROR_CRYPTO_INTERNAL;
    }
    // check for out-of-bounds write
    if((rs + r_len + s_len) > (rs + *rs_len)) {
        // prevented out-of-bounds write
        return NRF_ERROR_CRYPTO_INTERNAL;
    }

    memcpy(rs + r_len, cur, s_len);

    return NRF_SUCCESS;
}

// Encode two integers in DER format
// TAG + LENGTH needs 2 bytes
// if the highest bit of the integer is set we need an extra byte
#define  DER_OVERHEAD ((2 + 1) * 2)

ret_code_t nrf_crypto_backend_optiga_sign(
    void           * p_context,
    void     const * p_private_key,
    uint8_t  const * p_data,
    size_t           data_size,
    uint8_t        * p_signature)
{
    nrf_crypto_backend_optiga_ecc_private_key_t * p_prv =
            (nrf_crypto_backend_optiga_ecc_private_key_t *)p_private_key;

    optiga_key_id_t oid = p_prv->oid;

    uint16_t der_sig_len = NRF_CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE + DER_OVERHEAD;
    uint8_t der_sig[NRF_CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE + DER_OVERHEAD] = {0};

    optiga_crypt_ecdsa_sign((uint8_t *)p_data, data_size, oid, der_sig, &der_sig_len);

    size_t sig_len = NRF_CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE;
    int ret = asn1_to_ecdsa_rs(der_sig, der_sig_len, p_signature, &sig_len);
    
    return NRF_SUCCESS;
}

/**
 * @brief Encodes the ECDSA signature components (r, s) in ASN.1 format.
 *
 * @param[in]   r            Component r of the ECDSA signature
 * @param[in]   r_len        Length of the r component of the ECDSA signature
 * @param[in]   s            Component s of the ECDSA signature
 * @param[in]   s_len        Length of the s component of the ECDSA signature
 * @param[out]  asn_sig      Buffer where the resulting ASN.1-encoded ECDSA signature will be copied into
 * @param[out]  asn_sig_len  Length of the actual data that was copied into the output buffer
 * @returns     0 on success, -1 on error
 */
static int ecdsa_rs_to_asn1(
    const uint8_t  *r,
    size_t        r_len,
    const uint8_t  *s,
    size_t        s_len,
    uint8_t        *asn_sig,
    size_t       *asn_sig_len)
{
    uint32_t index;

    if(*asn_sig_len < (r_len + s_len + DER_OVERHEAD)) {
    	// not enough space in output buffer
    	return -1;
    }

    index = 0;

    // R component
    asn_sig[index + 0] = 0x02;
    asn_sig[index + 1] = 0x20;
    if (r[0] & 0x80)
    {
        asn_sig[index + 1] += 1;
        asn_sig[index + 2] =  0;
        index++;
    }
    memcpy(&asn_sig[index + 2], &r[0], r_len);
    index += r_len + 2;

    // S component
    asn_sig[index + 0] = 0x02;
    asn_sig[index + 1] = 0x20;
    if (s[0] & 0x80)
    {
        asn_sig[index + 1] += 1;
        asn_sig[index + 2] =  0;
        index++;
    }
    memcpy(&asn_sig[index + 2], &s[0], s_len);
    index += s_len + 2;

    *asn_sig_len = index; // Return total length of ASN.1-encoded data structure
    return 0;
}


ret_code_t nrf_crypto_backend_optiga_verify(
    void           * p_context,
    void     const * p_public_key,
    uint8_t  const * p_data,
    size_t           data_size,
    uint8_t  const * p_signature)
{
    nrf_crypto_backend_secp256r1_public_key_t * p_pub =
            (nrf_crypto_backend_secp256r1_public_key_t *)p_public_key;

    optiga_key_id_t oid = p_pub->oid;


    size_t der_sig_len = NRF_CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE + DER_OVERHEAD;
    uint8_t der_sig[NRF_CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE + DER_OVERHEAD] = {0};

    const size_t rs_size = NRF_CRYPTO_ECDSA_SECP256R1_SIGNATURE_SIZE/2;
    
    // convert signature to DER format needed by OPTIGA
    ecdsa_rs_to_asn1(p_signature, rs_size, p_signature + rs_size, rs_size, der_sig, &der_sig_len);

    optiga_lib_status_t res = optiga_crypt_ecdsa_verify((uint8_t *)p_data, data_size, der_sig, der_sig_len, 1, &oid);

    if(res != OPTIGA_LIB_SUCCESS) {
      return NRF_ERROR_CRYPTO_INTERNAL;
    }
   
    return NRF_SUCCESS;
}


#endif // NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)
