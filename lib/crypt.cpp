#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <string>

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <utility>

#include "libpassmg/crypt.hpp"
#include "libpassmg/utils.hpp"

std::unique_ptr<unsigned char[]> sha256(const std::string input) {
    auto md = std::make_unique<unsigned char[]>(SHA256_OUTPUT_LENGTH);

    SHA256((unsigned char *)input.c_str(), input.length(), md.get());

    return std::move(md);
}

std::string sha256_sum(const std::string data) {
    auto hashed = sha256(data);
    std::string hased_hex = to_hex(hashed.get(), SHA256_OUTPUT_LENGTH);

    return hased_hex;
}
