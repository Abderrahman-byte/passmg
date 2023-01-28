#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <utility>

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/types.h>

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

/* this implementation is inspired from openssl documentation
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption#C.2B.2B_Programs
 *
 * returns hex encoded encrypted data
 * */
std::unique_ptr<unsigned char[]> encrypt_aes_256(const std::string data,
                                                 const std::string passphrase) {
    int ciphertext_len, rc;
    int outlen = AES_OUTPUT_LENGTH(data.length());
    auto output = std::make_unique<unsigned char[]>(outlen);
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

    /* Hash the passphrase to get the encryption key */
    auto key = sha256(passphrase);

    if (ctx.get() == nullptr)
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");

    rc =
        EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_ecb(), NULL, key.get(), NULL);

    if (rc != 1) throw std::runtime_error("EVP_EncryptInit_ex failed");

    rc = EVP_EncryptUpdate(ctx.get(), output.get(), &outlen,
                           (unsigned char *)data.c_str(), data.length());

    ciphertext_len = outlen;

    if (rc != 1) throw std::runtime_error("EVP_EncryptUpdate failed");

    rc = EVP_EncryptFinal_ex(ctx.get(), output.get() + outlen, &ciphertext_len);

    return std::move(output);
}

std::string decrypt_aes_256(unsigned char *cipher, std::size_t ciphertext_len,
                            std::string passphrase) {
    unsigned char plain[ciphertext_len];
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc, plainlen, len;

    auto key = sha256(passphrase);

    memset(plain, 0, ciphertext_len);

    if (ctx.get() == nullptr)
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");

    rc =
        EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_ecb(), NULL, key.get(), NULL);

    if (rc != 1) throw std::runtime_error("EVP_DecryptInit_ex failed");

    rc = EVP_DecryptUpdate(ctx.get(), plain, &len, cipher, ciphertext_len);

    plainlen = len;

    if (rc != 1) throw std::runtime_error("EVP_DecryptUpdate failed");

    rc = EVP_DecryptFinal_ex(ctx.get(), plain + len, &len);

    if (rc != 1) throw std::runtime_error("EVP_DecryptFinal_ex()");

    plainlen += len;

    return std::string(plain, plain + plainlen);
}

std::string encrypt_aes_256_hex(const std::string data,
                                const std::string passphrase) {
    std::unique_ptr<unsigned char[]> encoded =
        encrypt_aes_256(data, passphrase);

    return to_hex(encoded.get(), AES_OUTPUT_LENGTH(data.length()));
}

std::string decrypt_aes_256_hex(std::string data_hex, std::string passphrase) {
    std::unique_ptr<unsigned char[]> cipher(from_hex(data_hex));
    std::size_t ciphertext_len = (data_hex.length() + 1) / 2;

    return decrypt_aes_256(cipher.get(), ciphertext_len, passphrase);
}
