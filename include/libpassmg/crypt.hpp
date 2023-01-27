#include <memory>
#include <string>

#include <openssl/sha.h>

#ifndef _PASSMG_CRYPT_HPP_
#define _PASSMG_CRYPT_HPP_

#define SHA256_OUTPUT_LENGTH (SHA256_DIGEST_LENGTH * sizeof(unsigned char))

extern std::unique_ptr<unsigned char[]> sha256(const std::string data);
extern std::string sha256_sum(const std::string data);
/* extern unsigned char *encrypt_aes_256(std::string, std::string, int *); */
/* extern std::string decrypt_aes_256(unsigned char *, int, std::string); */

#endif
