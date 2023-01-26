#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include <gtest/gtest.h>

#include <libpassmg/utils.hpp>

TEST(utils, file_exists) {
    EXPECT_ANY_THROW(file_exists(""));
    EXPECT_FALSE(file_exists("idontexits"));

    // FIXME create tempfile and check if it exists
    EXPECT_TRUE(file_exists("/usr/bin/ls"));
}

TEST(utils, hex) {
    std::array<std::string, 6> hex_data = {
        "68656c6c6f",
        "61626465727261686d616e65",
        "7468697320697320616e20617765736f6d65207468696e6720666f722064657673",
        "7fffffffffffffff",
        "834af3103feffaff",
        "0af0",
    };

    for (const auto &data : hex_data) {
        unsigned char *ascii_data = from_hex(data);
        std::string hex = to_hex(ascii_data, HTC_LENGTH(data.length()));

        EXPECT_EQ(hex.compare(data), 0);

        free(ascii_data);
    }
}

TEST(utils, random) {
    //
    GTEST_SKIP();
}
