#include <string>

#ifndef _CRYPT_PASS_H_
#define _CRYPT_PASS_H_

extern unsigned char* sha256(std::string);
extern unsigned char* encrypt_aes_256(std::string, std::string, int *);
#endif
