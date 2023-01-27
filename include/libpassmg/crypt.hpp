#include <memory>
#include <string>

#include <openssl/sha.h>

#ifndef _PASSMG_CRYPT_HPP_
#define _PASSMG_CRYPT_HPP_

#define SHA256_OUTPUT_LENGTH (SHA256_DIGEST_LENGTH * sizeof(unsigned char))
#define AES_OUTPUT_LENGTH(size) ((size / 16 + 1) * 16)

extern std::unique_ptr<unsigned char[]> sha256(const std::string data);
extern std::string sha256_sum(const std::string data);
extern std::unique_ptr<unsigned char[]>
encrypt_aes_256(const std::string data, const std::string passphrase);
extern std::string decrypt_aes_256(unsigned char *cipher,
                                   std::size_t ciphertext_len,
                                   std::string passphrase);
#endif
