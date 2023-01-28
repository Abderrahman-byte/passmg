#include <gtest/gtest.h>

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
