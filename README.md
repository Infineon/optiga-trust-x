# OPTIGA&trade; Trust X Software Framework

## Quick navigation

<a href="https://github.com/Infineon/optiga-trust-x#description"><img src="https://github.com/Infineon/Assets/blob/master/Pictures/optiga_trust_x_gitrepo_tile_1.jpg" width="256" height="192"></a> <a href="https://github.com/Infineon/optiga-trust-x#examples"><img src="https://github.com/Infineon/Assets/blob/master/Pictures/optiga_trust_x_gitrepo_tile_2.jpg" width="256" height="192"></a>  <a href="https://github.com/Infineon/optiga-trust-x#get-started"><img src="https://github.com/Infineon/Assets/blob/master/Pictures/optiga_trust_x_gitrepo_tile_3.jpg" width="256" height="192"></a>

<a href="https://github.com/Infineon/optiga-trust-x#documentation"><img src="https://github.com/Infineon/Assets/blob/master/Pictures/optiga_trust_x_gitrepo_tile_4.jpg" width="256" height="192"></a> <a href="https://github.com/Infineon/optiga-trust-x/wiki/Crypto-Performance"><img src="https://github.com/Infineon/Assets/blob/master/Pictures/optiga_trust_x_gitrepo_tile_5.jpg" width="256" height="192"></a>  <a href="https://github.com/Infineon/optiga-trust-x/wiki/Trust-X-Crypt-API"><img src="https://github.com/Infineon/Assets/blob/master/Pictures/optiga_trust_x_gitrepo_tile_6.jpg" width="256" height="192"></a>


## Security Chip

### <a name="description"></a>Description

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/OPTIGA-Trust-X.png">

This repository contains a target-agnostic Software Framework for the [OPTIGA™ Trust X](https://www.infineon.com/optiga-trust-x) security chip. It is a basis for other application notes.

### <a name="summary"></a>Summary
The [OPTIGA™ Trust X](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_Datasheet_v2.6.pdf) is a security solution based on a secure microntroller. Each device is shipped with a unique elliptic-curve keypair and a corresponding X.509 certificate. OPTIGA™ Trust X enables easy integration into existing PKI infrastructure.

### <a name="key_features_and_benefits"></a>Key Features and Benefits
* High-end security controller
* Turnkey solution
* One-way authentication using ECDSA
* Mutual authentication using DTLS client (IETF standard RFC 6347)
* Secure communication using DTLS
* Compliant with the USB Type-C™ Authentication standard
* I2C interface
* Up to 10 KB user memory
* Cryptographic support: ECC256, AES128, SHA-256, TRNG, DRNG
* PG-USON-10-2 package (3 x 3 mm)
* Standard & extended temperature ranges
* Full system integration support
* Common Criteria Certified EAL6+ (high) hardware
* Cryptographic Tool Box based on ECC NIST P256, P384 and SHA256 (sign, verify, key generation, ECDH, session key derivation)   

## Get Started

  * [Software Framework overview](#software_framework_oveview)
  * [Evaluation and developement kits](#evaluation_development_kits)
  * [Examples](#examples)
  * [Documentation](#documentation)

### <a name="software_framework_oveview"></a>Software Framework overview

![hostcode architecture](https://raw.githubusercontent.com/Infineon/Assets/master/Pictures/optiga_trust_x_stack_generic.jpg)

1. See [Trust X Crypt API](https://github.com/Infineon/optiga-trust-x/wiki/Trust-X-Crypt-API) and [Trust X Util API](https://github.com/Infineon/optiga-trust-x/wiki/Trust-X-Util-API)  to know more about optiga_crypt and optiga_util modules
2. Information about the OPTIGA™ Trust X Command Library can be found in the [Solution Reference Manual v1.35](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_SolutionReferenceManual_v1.35.pdf)
In the same document you can find explanation of all Object IDs (OIDs) available for users as well as detailed technical explanation for all features and envisioned use cases.
3. Infineon I<sup>2</sup>C Protocol implementation details can be found [here](https://github.com/Infineon/Assets/raw/master/PDFs/IFXI2CProtocol_v1.65.pdf)
4. Platform Abstraction Layer (PAL) overview and Porting Guide are presented in the [Wiki](https://github.com/Infineon/optiga-trust-x/wiki/Porting-Guide)

For more information please refer to the [Wiki page](https://github.com/Infineon/optiga-trust-x/wiki) of this project

### <a name="evaluation_development_kits"></a>Evaluation and developement kits
* OPTIGA™ Trust X [evaluation and demonstration kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-x-eval-kit/)
* OPTIGA™ Trust X [Shield2Go](https://www.infineon.com/cms/en/product/evaluation-boards/s2go-security-optiga-x/)

## <a name="examples"></a>Examples

1. [Get started guide](https://github.com/Infineon/getstarted-optiga-trust-x)
2. [On-Chip DTLS example](https://github.com/Infineon/onchipdtls-optiga-trust-x)
3. [Off-Chip TLS example (mbedTLS)](https://github.com/Infineon/mbedTLS-optiga-trust-x)
4. [AWS FreeRTOS example](https://github.com/Infineon/amazon-freertos-optiga-trust-x)
5. [Secure Firmware Update and Secure Boot](https://github.com/Infineon/fwupd-secboot-optiga-trust)
6. [Arduino library](https://github.com/Infineon/arduino-optiga-trust-x)
7. [Personalize OPTIGA™ Trust](https://github.com/Infineon/personalize-optiga-trust)
8. [OpenSSL Engine and Command Line Interface](https://github.com/Infineon/cli-optiga-trust-x)
9. [Python package](https://github.com/Infineon/python-optiga-trust)
10. [I2C Utilities](https://github.com/Infineon/i2c-utils-optiga-trust)
11. [OPTIGA Trust X Technical Training](https://github.com/Infineon/technical-training-optiga-trust-x)

## <a name="documentation"></a>Documentation

### Usefull articles

  * [Initialisation hints (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Initialisation-hints)
  * [Porting guide (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Porting-Guide)
  * [Crypto performance (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Crypto-Performance)
  * [Error Codes (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Device-Error-Codes)
  * [Metadata and Access Conditions (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Metadata-and-Access-Conditions)
  * [On‐chip vs off-chip DTLS Protocol (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/On‐chip-vs-off‐chip-(D)TLS-Protocol)
  * **User API**
      * [Crypt API (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Trust-X-Crypt-API)
      * [Util API (Wiki)](https://github.com/Infineon/optiga-trust-x/wiki/Trust-X-Util-API)
      
  * [Hardware-Security: "Einfach (und) Sicher"](https://vimeo.com/279839814) in German, [Slides](https://github.com/Infineon/Assets/blob/master/PDFs/2018-06-04_Building-IoT_HW-Sec_Lesjak_vFinal.pdf) in English

### Datasheet and Co.

For high level description and some important excerpts from the documentation please refer to [Wiki page](https://github.com/Infineon/optiga-trust-x/wiki)

Other downloadable PDF documents can be found below:
1. [OPTIGA™ Trust X Datasheet v2.6](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_Datasheet_v2.6.pdf) (PDF)
2. [OPTIGA™ Solution Reference Manual v1.35](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_SolutionReferenceManual_v1.35.pdf) (PDF)
3. [OPTIGA™ Keys and Certificates v1.2](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_KeysAndCertificates_v1.2.pdf) (PDF)
4. [Infineon I2C protocol specification v1.65](https://github.com/Infineon/Assets/raw/master/PDFs/IFXI2CProtocol_v1.65.pdf) (PDF)

### Board assembly recommendations

If you are planning to integrate OPTIGA™ Trust X in your PCB design have a look at the recommendations found [here](https://www.infineon.com/dgdl/Infineon-Board+Assembly+Recommendations-Discrete+Leadless-Package-v06_00-EN.pdf?fileId=5546d4626cb27db2016cbda27a9c544d).




## <a name="contributing"></a>Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## <a name="license"></a>License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
