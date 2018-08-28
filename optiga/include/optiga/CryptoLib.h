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
* \brief   This file defines APIs, types and data structures for Crypto Wrapper.
*
* \ingroup  grCryptoLibWrapper
* @{
*/
#ifndef _CRYPTO_LIB_H_
#define _CRYPTO_LIB_H_

#include <stdint.h>
#include "optiga/common/Datatypes.h"
#include "optiga/common/CommandLib.h"
/****************************************************************************
 *
 * Hash Defines.
 *
 ****************************************************************************/



///Requested operation completed without any error
#define CRYPTO_LIB_OK                               0x76428903

///Null parameter(s)
#define CRYPTO_LIB_NULL_PARAM                       0x80003001

///Certificate parse failure
#define CRYPTO_LIB_CERT_PARSE_FAIL                  (CRYPTO_LIB_NULL_PARAM + 1)

///Signature Verification failure
#define CRYPTO_LIB_VERIFY_SIGN_FAIL                 (CRYPTO_LIB_NULL_PARAM + 2)

///SHA256 generation failure
#define CRYPTO_LIB_SHA256_FAIL                      (CRYPTO_LIB_NULL_PARAM + 3)

///Length of input is zero
#define CRYPTO_LIB_LENZERO_ERROR                    (CRYPTO_LIB_NULL_PARAM + 4)

///Length of Parameters are zero
#define CRYPTO_LIB_LENMISMATCH_ERROR                (CRYPTO_LIB_NULL_PARAM + 5)

///Memory allocation failure
#define CRYPTO_LIB_MEMORY_FAIL                      (CRYPTO_LIB_NULL_PARAM + 6)

///Insufficient memory
#define CRYPTO_LIB_INSUFFICIENT_MEMORY              (CRYPTO_LIB_NULL_PARAM + 7)

///Generic error condition
#define CRYPTO_LIB_ERROR                            0xF1743903

/****************************************************************************
 *
 * Common data structure used across all functions.
 *
 ****************************************************************************/

/**
 * \brief Structure which holds parsed certificate data.
 */
typedef struct sCertificate_d
{
    ///Public Key
    sbBlob_d sPublicKey;
    
    ///Certificate Signature
    sbBlob_d sCertSignature;
    
    ///Certificate Data start
    sbBlob_d sCertData;
}sCertificate_d;

/**
 * \brief Structure which holds data for signature verification
 */
typedef struct sSignatureVector_d
{
    ///Public Key
    sbBlob_d *psPublicKey;
    
    /// Signature
    sbBlob_d *psSignature;
    
    /// Message
    sbBlob_d *psMessage;
}sSignatureVector_d;


/****************************************************************************
 *
 * Definitions for Crypto Libray APIs.
 *
 ****************************************************************************/
/**
 * \brief Parses raw X509 v3 certificate into a custom defined certificate structure.
 */
LIBRARY_EXPORTS int32_t  CryptoLib_ParseCertificate(const sbBlob_d *PpsRawCertificate,sCertificate_d *PpsCertificate);

/**
 * \brief Verifies the signature using the given public key.
 */
LIBRARY_EXPORTS int32_t  CryptoLib_VerifySignature(const sSignatureVector_d *PpsSignatureVector);

/**
 * \brief Gets Random bytes of data for a given length.
 */
LIBRARY_EXPORTS int32_t  CryptoLib_GetRandom(uint16_t PwRandomDataLength,sCmdResponse_d *PpsResponse);

/**
 * \brief Generates seed by concatenating random and the counter and hashing(SHA256) the concatenated value.
 */
LIBRARY_EXPORTS int32_t CryptoLib_GenerateSeed(puint8_t PpbSeed, uint32_t PdwSeedLength);

/// @cond hidden
/** 
 * \brief Gets the crypto version number along with build number
 */
 LIBRARY_EXPORTS int32_t CryptoLib_WolfSSlVersion(sbBlob_d *PpsVersion);
/// @endcond 

#endif //_CRYPTO_LIB_H_






























