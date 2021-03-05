#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <exception>
#include <string.h>

#include <openssl/conf.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "utils.h"

unsigned char* sha256(std::string input) {
	unsigned char *hashed; // Hashed binary data
	hashed = (unsigned char *)malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char));

	SHA256_CTX hash_ctx; /* Sha256 context where data of the operations is stored */
	SHA256_Init(&hash_ctx); /* Init sha256 context */
	SHA256_Update(&hash_ctx, input.c_str(), input.size()); /* Hash data */
	SHA256_Final(hashed, &hash_ctx); /* Finish hasing */
	
	return hashed;
}

unsigned char* encrypt_aes_256(std::string data, std::string passphrase, int *ciphertext_len) {
	unsigned char *key, *plaintext, ciphertext[2048], *output;
	EVP_CIPHER_CTX *ctx; /* Cipher context to store operation data */
	int len;
	
	plaintext = (unsigned char *)data.c_str(); // data to binary data
	key = sha256(passphrase); /* Hash the passphrase to be encryption key */

	if((ctx = EVP_CIPHER_CTX_new()) == NULL) /* Init cipher context */
		throw std::runtime_error("EVP_CIPHER_CTX_new()");

	if(EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL) != 1)
		throw std::runtime_error("EVP_EncryptInit_ex()");

	if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, data.length()) != 1)
		throw std::runtime_error("EVP_EncryptUpdate()");
	
	*ciphertext_len = len;

	if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
		throw std::runtime_error("EVP_EncryptFinal_ex()");

	*ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	output = (unsigned char *)malloc(sizeof(unsigned char) * *ciphertext_len);
	memcpy((void *)output, (void *)ciphertext, *ciphertext_len);

	return output;
}

std::string decrypt_aes_256(unsigned char *ciphertext, int ciphertext_len, std::string passphrase) {
	std::string output;
	unsigned char *key, plaintext[2048];
	int len, plaintext_len;
	EVP_CIPHER_CTX *ctx;
	
	key = sha256(passphrase); /* Hash passphrase to use it as encryption/decryption key */

	if((ctx = EVP_CIPHER_CTX_new()) == NULL) /* Init cipher context */ 
		throw std::runtime_error("EVP_CIPHER_CTX_new()");

	if(EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL) != 1) 
		throw std::runtime_error("EVP_DecryptInit_ex()");

	if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
		throw std::runtime_error("EVP_DecryptUpdate()");

	plaintext_len = len;
	
	if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
		// throw std::runtime_error("EVP_DecryptFinal_ex()");

	plaintext_len += len;
	
	EVP_CIPHER_CTX_free(ctx);	
	
	output = std::string((char *)plaintext, plaintext_len);
	return output;
}
