#include <stdio.h>
#include <string.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <bitcoin_wallet_ta.h>
#include <btc.h>

#define NODE_ADDRESS 0xC0001000 // define the start address of the memory region

HDNode node;

static TEE_Result check_masterkey(uint32_t param_types, TEE_Param params[4]);
static TEE_Result generate_new_masterkey(uint32_t param_types, TEE_Param params[4]);
static TEE_Result mnemonic_to_masterkey(uint32_t param_types, TEE_Param params[4]);
static TEE_Result erase_masterkey(uint32_t param_types, TEE_Param params[4]);
static TEE_Result sign_transaction(uint32_t param_types, TEE_Param params[4]);
static TEE_Result get_bitcoin_address (uint32_t param_types, TEE_Param params[4]);

TEE_Result TA_CreateEntryPoint(void){
	DMSG("has been called");
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void){
	DMSG("has been called");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types, TEE_Param params[4], void **sess_ctx){

	DMSG("has been called");
  (void)&param_types;
	(void)&params;
	(void)&sess_ctx;
	IMSG("Bitcoin Wallet\n");
  memcpy(&node, (void*)NODE_ADDRESS, sizeof(HDNode));
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ctx){
	(void)&sess_ctx;

  memcpy((void*)NODE_ADDRESS, &node, sizeof(HDNode));
	IMSG("Goodbye!\n");
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id, uint32_t param_types, TEE_Param params[4]){

	(void)&sess_ctx;
	
	IMSG("Choice from NW: %d\n",cmd_id);
	switch (cmd_id) {
		case TA_BITCOIN_WALLET_CMD_1:
			return check_masterkey(param_types, params);
		case TA_BITCOIN_WALLET_CMD_2:
			return generate_new_masterkey(param_types, params);
		case TA_BITCOIN_WALLET_CMD_3:
			return mnemonic_to_masterkey(param_types, params);
		case TA_BITCOIN_WALLET_CMD_4:
			return erase_masterkey(param_types, params);
		case TA_BITCOIN_WALLET_CMD_5:
			return sign_transaction(param_types, params);
		case TA_BITCOIN_WALLET_CMD_6:
			return get_bitcoin_address(param_types, params);
		default:
			return TEE_ERROR_BAD_PARAMETERS;
	}
}

static TEE_Result check_masterkey(uint32_t param_types, TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(	TEE_PARAM_TYPE_VALUE_INOUT, 
												TEE_PARAM_TYPE_VALUE_OUTPUT, 
												TEE_PARAM_TYPE_NONE, 
												TEE_PARAM_TYPE_NONE);
	
	uint32_t res;

	DMSG("has been called");

	// check parameter types
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	// Check pin
	if(params[0].value.a!=1234){
		params[0].value.b = 2;
		return TEE_SUCCESS;
	}

	res = btc_check_masterkey(&node);
  
	if(res == 0){
		params[1].value.a = 1;
		DMSG("Master Key exists");
	}
	else {
		params[1].value.a = 0;
		DMSG("Master Key does not exist");
	}

#ifdef DEBUG_MODE
  DMSG("Public Key: ");
  for(int i=0; i<32; i++){
    printf("%x", node.public_key[i]);
  }
  printf("\r\n");
  DMSG("Private Key: ");
  for(int i=0; i<32; i++){
    printf("%x", node.private_key[i]);
  }
  printf("\r\n");
  DMSG("Chain code: ");
  for(int i=0; i<32; i++){
    printf("%x", node.chain_code[i]);
  }
  printf("\r\n");
#endif
	return TEE_SUCCESS;
}

static TEE_Result generate_new_masterkey(uint32_t param_types, TEE_Param params[4]){

	uint32_t exp_param_types = TEE_PARAM_TYPES(	TEE_PARAM_TYPE_VALUE_INOUT, 
												TEE_PARAM_TYPE_MEMREF_OUTPUT, 
												TEE_PARAM_TYPE_NONE, 
												TEE_PARAM_TYPE_NONE);
	DMSG("has been called");

	// check parameter types
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	// Check pin
	if(params[0].value.a!=1234){
		params[0].value.b = 2;
		return TEE_SUCCESS;
	}
	
 // Generate a new mnemonic
    const char *new_mnemonic = btc_mnemonic_generate(MNEMONIC_STRENGTH); // 128 bits of entropy for a 12-word mnemonic

    // Ensure mnemonic generation was successful
    if (new_mnemonic == NULL) {
        return 2; // Error: Failed to generate mnemonic
    }

    // Convert the mnemonic to a master key
    btc_mnemonic_to_masterkey(&node, (char *)new_mnemonic);
    memcpy(params[1].memref.buffer, new_mnemonic, 128);
    params[1].memref.size = 128;

#ifdef DEBUG_MODE
    IMSG("Generated mnemonic: %s\r\n", new_mnemonic);
	  IMSG("\r\n");
    IMSG("Private Key: ");
    for(int i=0; i<32; i++){
		  printf("%x", node.private_key[i]);
	  }
	  IMSG("\r\n");
    IMSG("Chain code: ");
    for(int i=0; i<32; i++){
		  printf("%x", node.chain_code[i]);
	  }
	  IMSG("\r\n");
#endif

	return TEE_SUCCESS;
}

static TEE_Result mnemonic_to_masterkey(uint32_t param_types, TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(	TEE_PARAM_TYPE_VALUE_INOUT, 
												TEE_PARAM_TYPE_MEMREF_INPUT, 
												TEE_PARAM_TYPE_NONE, 
												TEE_PARAM_TYPE_NONE);
	DMSG("has been called");

	// check parameter types
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	// Check pin
	if(params[0].value.a!=1234){
		params[0].value.b = 2;
		return TEE_SUCCESS;
	}
  
  btc_mnemonic_to_masterkey(&node, (char *)params[1].memref.buffer);

	return TEE_SUCCESS;
}

static TEE_Result erase_masterkey(uint32_t param_types, TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(	TEE_PARAM_TYPE_VALUE_INOUT, 
												TEE_PARAM_TYPE_VALUE_OUTPUT, 
												TEE_PARAM_TYPE_NONE, 
												TEE_PARAM_TYPE_NONE);
	DMSG("has been called");

	// check parameter types
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	// Check pin
	if(params[0].value.a!=1234){
		params[0].value.b = 2;
		return TEE_SUCCESS;
	}
  
  btc_erase_masterkey(&node);

	return TEE_SUCCESS;
}

static TEE_Result get_bitcoin_address(uint32_t param_types, TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(	TEE_PARAM_TYPE_VALUE_INOUT, 
												TEE_PARAM_TYPE_VALUE_INPUT, 
												TEE_PARAM_TYPE_MEMREF_OUTPUT, 
												TEE_PARAM_TYPE_NONE);
	uint8_t address[36];
	
	DMSG("has been called");

	// check parameter types
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	// Check pin
	if(params[0].value.a!=1234){
		params[0].value.b = 2;
		return TEE_SUCCESS;
	}

	uint8_t account_id = params[1].value.a;
  (void) account_id;
	IMSG("%d\r\n",account_id);

	btc_get_bitcoin_address(&node, (uint8_t *) address, 36);

  IMSG("Bitcoin address: %s\r\n", address);

	TEE_MemMove(params[2].memref.buffer, address, 36);
  params[2].memref.size = 36;

	return TEE_SUCCESS;
}

static TEE_Result sign_transaction(uint32_t param_types, TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(	TEE_PARAM_TYPE_VALUE_INOUT, 
												TEE_PARAM_TYPE_MEMREF_INPUT, 
												TEE_PARAM_TYPE_MEMREF_OUTPUT,
												TEE_PARAM_TYPE_VALUE_INPUT);
	uint32_t res = 0;

	DMSG("has been called");

	// check parameter types
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	// Check pin
	if(params[0].value.a!=1234){
		params[0].value.b = 2;
		return TEE_SUCCESS;
	}

	//account_id = params[3].value.a;

  // Sign a raw transaction
  uint8_t rawtx[256];
  uint8_t signature[MAX_SIGNATURE_SIZE];
  btc_sign_raw_tx(&node, rawtx, 256, signature);
  
  IMSG("Signature: ");
  for (uint32_t i = 0; i < 64; i++){
    IMSG("%x", signature[i]);
  }
  IMSG("\r\n");

  params[2].memref.size = 64;

	if (res == 0) {
		DMSG("Transaction has been succefully signed.");
		params[0].value.a = 1;
	} else {
		DMSG("Failed to sign address. Code 0x%x\n", res);
		params[0].value.b = 1;
	}

	return TEE_SUCCESS;
}

