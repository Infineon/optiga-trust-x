# Infineon OPTIGA&trade; Trust X Application Note Framework

[tocstart]: # (toc start)

* [Description](#description)
* [Summary](#summary)
* [Evaluation and developement kits](#key_features_and_benefits)
* [Get Started](#get_started)
* [Overview](#oveview)
* [Documentation](#documentation)
* [Porting Guide](#porting_guide)
* [Contributing](#contributing)
* [License](#license)

[tocend]: # (toc end)

## <a name="description"></a>Description

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/OPTIGA-Trust-X.png">

This repository contains a target-agnostic Application Notes Framework for the [OPTIGA™ Trust X](https://www.infineon.com/optiga-trust-x) security chip. It is a basis for other application notes.

## <a name="summary"></a>Summary
The [OPTIGA™ Trust X](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_Datasheet_v2.5.pdf) is a security solution based on a secure microntroller. Each device is shipped with a unique elliptic-curve keypair and a corresponding X.509 certificate. OPTIGA™ Trust X enables easy integration into existing PKI infrastructure.

## <a name="key_features_and_benefits"></a>Key Features and Benefits
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

## <a name="key_features_and_benefits"></a>Evaluation and developement kits
* OPTIGA™ Trust X [evaluation and demonstration kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-x-eval-kit/)
* OPTIGA™ Trust X [Shield2Go](https://www.infineon.com/cms/en/product/evaluation-boards/s2go-security-optiga-x/)

## <a name="get_started"></a>Get Started
1. [Get started guide](https://github.com/Infineon/getstarted-optiga-trust-x)
2. [On-Chip DTLS example](https://github.com/Infineon/onchipdtls-optiga-trust-x)
3. [Off-Chip TLS example (mbedTLS)](https://github.com/Infineon/mbedTLS-optiga-trust-x)
4. [AWS FreeRTOS example](https://github.com/Infineon/amazon-freertos-optiga-trust-x)
5. [Arduino library](https://github.com/Infineon/arduino-optiga-trust-x)
6. [Personalisation example](https://github.com/Infineon/personalize-optiga-trust-x)

## <a name="oveview"></a>Overview

![hostcode architecture](https://raw.githubusercontent.com/Infineon/Assets/master/Pictures/optiga_trust_x_stack_generic.jpg)

1. See [OPTIGA Crypt API](https://github.com/Infineon/optiga-trust-x/wiki/OPTIGA-Crypt-API) and [OPTIGA Util API](https://github.com/Infineon/optiga-trust-x/wiki/OPTIGA-Util-API) to know more about optiga_crypt and optiga_util modules
2. Information about the OPTIGA™ Trust X Command Library can be found in the [Solution Reference Manual v1.35](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_SolutionReferenceManual_v1.35.pdf)
In the same document you can find explanation of all Object IDs (OIDs) available for users as well as detailed technical explanation for all features and envisioned use cases.
3. Infineon I<sup>2</sup>C Protocol implementation details can be found [here](https://github.com/Infineon/Assets/raw/master/PDFs/IFXI2CProtocol_v1.65.pdf)
4. Platform Abstraction Layer (PAL) overview and Porting Guide are presented in the [Wiki](https://github.com/Infineon/optiga-trust-x/wiki/Porting-Guide)

For more information please refer to the [Wiki page](https://github.com/Infineon/optiga-trust-x/wiki) of this project

## <a name="documentation"></a>Documentation
For high level description and some important excerpts from the documentation please refer to [Wiki page](https://github.com/Infineon/optiga-trust-x/wiki)

Other downloadable PDF documents can be found below:
1. [OPTIGA™ Trust X Datasheet v2.5](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_Datasheet_v2.5.pdf) (PDF)
2. [OPTIGA™ Solution Reference Manual v1.35](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_SolutionReferenceManual_v1.35.pdf) (PDF)
3. [OPTIGA™ Keys and Certificates v1.2](https://github.com/Infineon/Assets/raw/master/PDFs/OPTIGA_Trust_X_KeysAndCertificates_v1.2.pdf) (PDF)
4. [Infineon I<sup>2</sup>C protocol specification v1.65](https://github.com/Infineon/Assets/raw/master/PDFs/IFXI2CProtocol_v1.65.pdf) (PDF)

## <a name="porting_guide"></a>Porting Guide
You might find a list of supported Platforms [here](https://github.com/Infineon/optiga-trust-x/tree/master/pal). These are:
* Espressif ESP32 with Amazon freeRTOS
* Espressif ESP32 with freeRTOS
* Nordic NRF52
* Raspberry Pi (Embedded Linux)
* Infineon XMC microcontrollers

For other platform please refer to our porting guide on [Wiki](https://github.com/Infineon/optiga-trust-x/wiki/Porting-Guide) page

## <a name="contributing"></a>Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## <a name="license"></a>License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
