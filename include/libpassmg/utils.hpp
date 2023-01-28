#include <cstddef>
#include <string>

#ifndef _UTILS_PASSMG_HPP_
#define _UTILS_PASSMG_HPP_

/* Ascii characteres to generate random strings */
#define ASCII_CHARS \
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

#define HTC_LENGTH(len) (len / 2) * sizeof(unsigned char)

static std::string ascii = ASCII_CHARS;

extern bool file_exists(const std::string filename);
extern std::string random_str(std::size_t length);
extern std::string to_hex(const unsigned char *, std::size_t size);
extern unsigned char *from_hex(const std::string &hex_data);

#endif
