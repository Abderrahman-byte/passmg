#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>

unsigned char* sha256(std::string input) {
	unsigned char *hashed; // Hashed binary data
	hashed = (unsigned char *)malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char));

	SHA256_CTX hash_ctx; /* Sha256 context where data of the operations is stored */
	SHA256_Init(&hash_ctx); /* Init sha256 context */
	SHA256_Update(&hash_ctx, input.c_str(), input.size()); /* Hash data */
	SHA256_Final(hashed, &hash_ctx); /* Finish hasing */
	
	/* std::stringstream output;
	
	for(int i = 0 ; i < SHA256_DIGEST_LENGTH; i++) {
		output << std::hex << std::setw(2) << std::setfill('0') << (int)hashed[i];
	} */

	return hashed;
}
