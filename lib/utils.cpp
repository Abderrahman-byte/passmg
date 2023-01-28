#include <cstddef>
#include <cstring>
#include <sys/stat.h>

#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>

#include "libpassmg/utils.hpp"

bool file_exists(const std::string filepath) {
    if (filepath.length() <= 0)
        throw std::invalid_argument("empty filepath argument");

    struct stat buff;

    return stat(filepath.c_str(), &buff) == 0;
}

std::string random_str(std::size_t len) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, ascii.length());
    std::string rnd_str;

    for (std::size_t i = 0; i < len; i++) {
        rnd_str += ascii[distribution(generator)];
    }

    return rnd_str;
}

std::string to_hex(const unsigned char *data, std::size_t size) {
    std::stringstream ss;

    for (int i = 0; i < size; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }

    return ss.str();
}

int parse_hex_digit(char digit) {
    unsigned char n;

    switch (digit) {
    case '0':
        n = 0;
        break;
    case '1':
        n = 1;
        break;
    case '2':
        n = 2;
        break;
    case '3':
        n = 3;
        break;
    case '4':
        n = 4;
        break;
    case '5':
        n = 5;
        break;
    case '6':
        n = 6;
        break;
    case '7':
        n = 7;
        break;
    case '8':
        n = 8;
        break;
    case '9':
        n = 9;
        break;
    case 'a':
        n = 10;
        break;
    case 'b':
        n = 11;
        break;
    case 'c':
        n = 12;
        break;
    case 'd':
        n = 13;
        break;
    case 'e':
        n = 14;
        break;
    case 'f':
        n = 15;
        break;
    }

    return n;
}

// FIXME probable data leak
unsigned char *from_hex(const std::string &hex_data) {
    std::size_t data_size = HTC_LENGTH(hex_data.length());
    unsigned char *data = new unsigned char[data_size];

    memset(data, 0, data_size);

    for (std::size_t i = 0; i < hex_data.length() - 1; i += 2) {
        unsigned char byte;
        byte = parse_hex_digit(hex_data[i]);
        byte = byte << 4;
        byte += parse_hex_digit(hex_data[i + 1]);
        data[i / 2] = byte;
    }

    return data;
}
