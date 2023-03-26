#include <btc.h>
#include <string.h>
#include <stdio.h>

#include "trezor-crypto/base58.h"
#include "trezor-crypto/bip32.h"
#include "trezor-crypto/bip39.h"
#include "trezor-crypto/curves.h"
#include "trezor-crypto/memzero.h"
#include "trezor-crypto/secp256k1.h"
#include "trezor-crypto/ripemd160.h"


#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

// Function to check if a master key is valid
uint32_t btc_check_masterkey(HDNode *node) {
    // Ensure the node is not NULL
    if (node == NULL) {
        return -1; // Error: Null node pointer
    }

    // Check if the private key is all zeros
    for(int i = 0; i < 32; i++) {
        if (node->private_key[i] != 0) {
            return 0; // Success: Private key is not zeros
        }
    }

  return -1; // Error: Private key is all zeros
}

// Function to generate a new master key
const char *btc_mnemonic_generate(int strength) {
    // Generate a new mnemonic
  return mnemonic_generate(strength); // 128 bits of entropy for a 12-word mnemonic
}

// Function to convert a mnemonic to a master key
uint32_t btc_mnemonic_to_masterkey(HDNode *node, char *mnemonic) {
    // Ensure the node is not NULL
    if (node == NULL) {
        return -1; // Error: Null node pointer
    }

    const char *passphrase = "trezor-wallet"; // You can provide an optional passphrase for added security
    uint8_t seed[64]; // Seed generated from mnemonic, used for deriving master key

    // Generate the seed from the mnemonic
    mnemonic_to_seed(mnemonic, passphrase, seed, NULL);


    // Derive the master key from the seed
    if (hdnode_from_seed(seed, sizeof(seed), SECP256K1_NAME, node) != 1) {
        return 2; // Failed to derive master key
    }

    return 0; // Success
}


// Function to erase the master key
uint32_t btc_erase_masterkey(HDNode *node) {
    // Ensure the node is not NULL
    if (node == NULL) {
        return 1; // Error: Null node pointer
    }

    // Securely erase the private key
    memzero(node->private_key, sizeof(node->private_key));

    // Securely erase the chain code
    memzero(node->chain_code, sizeof(node->chain_code));
    return 0; // Success
}

// Function to sign a raw transaction
uint32_t btc_sign_raw_tx(HDNode *node, uint8_t *rawtx, size_t bytes, uint8_t *sig) {
    if (node == NULL || rawtx == NULL || bytes == 0 || sig == NULL) {
        return -1;
    }

    uint8_t hash[SHA256_DIGEST_LENGTH];

    // Double SHA-256 hash of the raw transaction
    SHA256_CTX sha256_ctx;
    sha256_Init(&sha256_ctx);
    sha256_Update(&sha256_ctx, rawtx, bytes);
    sha256_Final(&sha256_ctx, hash);
    sha256_Init(&sha256_ctx);
    sha256_Update(&sha256_ctx, hash, SHA256_DIGEST_LENGTH);
    sha256_Final(&sha256_ctx, hash);

      // Sign the hash using the HDNode's private key
    int ret = ecdsa_sign_digest(&secp256k1, node->private_key, hash, sig, NULL, NULL);
    return ret;
}

// Function to get the Bitcoin address from an HDNode
void btc_get_bitcoin_address(HDNode *node, uint8_t *addr, size_t addr_size) {
    uint8_t hash[SHA256_DIGEST_LENGTH];
    hdnode_fill_public_key(node);
    sha256_Raw(node->public_key, 33, hash);
    ripemd160(hash, SHA256_DIGEST_LENGTH, hash);
    base58_encode_check(hash, RIPEMD160_DIGEST_LENGTH, 0x00, (char *) addr, addr_size);
}