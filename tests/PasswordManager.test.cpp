#include <gtest/gtest.h>
#include <iostream>

#include "libpassmg/PasswordManager.hpp"
#include "libpassmg/exceptions.hpp"

TEST(PasswordManager, exceptions) {
    ASSERT_THROW({ PasswordManager passmg("/memory:"); }, DatabaseException);
    ASSERT_THROW(
        { PasswordManager passmg("/file://darkstar/home/fred/data.db"); },
        DatabaseException);
    ASSERT_NO_THROW({ PasswordManager passmg(":memory:"); });

    ASSERT_THROW(
        {
            PasswordManager passmg(":memory:");
            passmg.signup("lsi", "123456");
        },
        InvalidInputException);

    ASSERT_THROW(
        {
            PasswordManager passmg(":memory:");
            passmg.signup("abderrahmane", "12345");
        },
        InvalidInputException);

    ASSERT_THROW(
        {
            PasswordManager passmg(":memory:");
            passmg.create("abderrahmane", "12345");
        },
        AuthenticationRequired);
}

TEST(PasswordManager, authentication) {
    PasswordManager passmg(":memory:");

    ASSERT_THROW(passmg.login("abderrahmane", "password");
                 , WrongCredentialsException);

    ASSERT_FALSE(passmg.is_authenticated());

    passmg.signup("abderrahmane", "password");

    ASSERT_TRUE(passmg.is_authenticated());

    ASSERT_THROW(passmg.login("abderrahmane", "1234567");
                 , WrongCredentialsException);

    ASSERT_FALSE(passmg.is_authenticated());

    passmg.login("abderrahmane", "password");

    ASSERT_TRUE(passmg.is_authenticated());

    ASSERT_THROW(passmg.signup("abderrahmane", "password");
                 , IntegrityException);
}

TEST(PasswordManager, CRUD) {
    PasswordManager passmg(":memory:");

    passmg.signup("username", "password");

    EXPECT_EQ(passmg.list().size(), 0);

    passmg.create("facebook", "face001");
    passmg.create("github", "1234567890");
    passmg.create("house key", "1234567890");

    auto pw1 = passmg.get(1);
    auto pw2 = passmg.get("github");
    auto pw3 = passmg.get("pass");

    EXPECT_EQ(pw1.title.compare("facebook"), 0);
    EXPECT_EQ(pw1.content.compare("face001"), 0);
    EXPECT_EQ(pw2.title.compare("github"), 0);
    EXPECT_EQ(pw2.content.compare("1234567890"), 0);
    EXPECT_EQ(pw3.id, 0);
    EXPECT_EQ(passmg.list().size(), 3);

    passmg.remove(2);
    passmg.remove("house key");

    EXPECT_EQ(passmg.list().size(), 1);

    passmg.signup("username2", "password");

    EXPECT_EQ(passmg.list().size(), 0);

    passmg.login("username", "password");

    EXPECT_EQ(passmg.list().size(), 1);
}
