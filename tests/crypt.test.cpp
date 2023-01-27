#include <string>
#include <unordered_map>

#include "gtest/gtest.h"

#include "libpassmg/crypt.hpp"
#include "libpassmg/utils.hpp"

TEST(crypt, sha256) {
    std::unordered_map<std::string, std::string> data_hash = {
        {"abderrahmane",
         "46aaee26a5f0bf1cf9550caa32062c980d5f367a8a333ac881e63c23798a3255"},
        {"",
         "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
        {"1234567890",
         "c775e7b757ede630cd0aa1113bd102661ab38829ca52a6422ab782862f268646"}
    };

    for (const auto &kv : data_hash) {
        EXPECT_EQ(sha256_sum(kv.first).compare(kv.second), 0);
    }
}
