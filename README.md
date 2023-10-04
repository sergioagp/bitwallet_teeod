# Bitcoin wallet GP TEE Applicaiton
This repository contains a Bitcoin wallet application, structured into two components: the client app and the trusted application (TA). The client app is located in the host folder, and the TA can be found in the ta folder.

## Compilation
To compile the Bitcoin wallet application, follow these steps:

Specify the necessary compiler paths for the host and TA. If specific compilers are not specified, the CROSS_COMPILE variables will be used.

TEEC_EXPORT: Path to the TEEOD library.
HOST_CROSS_COMPILE: Path to the host compiler.
TA_CROSS_COMPILE: Path to the TA compiler.

Compile the Bitcoin wallet application using the provided Makefile:
```
make all
```
To clean up the compiled files, you can use the following command:
```
make clean
```
To copy the compiled files to the specified SD card path, use the following command:

## Usage
After deploying the Bitcoin wallet application on the Trusted Execution Environment (TEE), you can run the following commands:

To check if the master key exists:
```
./bitcoin_wallet 1 1234
```
To generate the master key:
```
./bitcoin_wallet 2 1234
```
To generate the master key from a given mnemonic:
```
./bitcoin_wallet 3 1234 -a [mnemonic]
```
Replace [mnemonic] with your mnemonic phrase.

To erase the master key:
```
./bitcoin_wallet 4 1234
```
To sign a transaction with a specific account number:
```
./bitcoin_wallet 5 1234 -a [account number]
```
Replace [account number] with the desired account number.

To get the public key:
```
./bitcoin_wallet 6 1234
```
These commands allow you to interact with the Bitcoin wallet application running on the TEE. Make sure to replace 1234 with your PIN code and provide any required arguments as specified.

For additional information and usage details, refer to the documentation provided with the application.
