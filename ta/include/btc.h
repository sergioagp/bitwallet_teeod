#ifndef __BTC_H__
#define __BTC_H__

#include <trezor-crypto/bip32.h>

#define MAX_SIGNATURE_SIZE 72
#define MNEMONIC_STRENGTH 128
#define BIP32_SERIALIZED_LEN  112 //78


// Bitcoin wallet commands
uint32_t btc_check_masterkey(HDNode *node);

const char *btc_mnemonic_generate(int strength);

uint32_t btc_mnemonic_to_masterkey(HDNode *node, char *mnemonic);

uint32_t btc_erase_masterkey(HDNode *node);

uint32_t btc_sign_raw_tx(HDNode *node, uint8_t *rawtx, size_t bytes, uint8_t *sig);

void     btc_get_bitcoin_address(HDNode *node, uint8_t *addr, size_t addr_size);

#endif /* __BTC_H__ */