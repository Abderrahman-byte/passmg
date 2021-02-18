#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>

std::string sha256(std::string input) {
	unsigned char hashed[SHA256_DIGEST_LENGTH];
	SHA256_CTX hash;
	SHA256_Init(&hash);
	SHA256_Update(&hash, input.c_str(), input.size());
	SHA256_Final(hashed, &hash);
	
	std::stringstream output;
	
	for(int i = 0 ; i < SHA256_DIGEST_LENGTH; i++) {
		output << std::hex << std::setw(2) << std::setfill('0') << (int)hashed[i];
	}

	return output.str();
}
