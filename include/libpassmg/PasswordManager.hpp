#include <cstddef>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "libpassmg/types.hpp"

#ifndef __PASSWORD_MANAGER__
#define __PASSWORD_MANAGER__

class PasswordManager {
    public:
        PasswordManager(std::string dbname);
        PasswordManager() = delete;
        ~PasswordManager();

        void login(std::string username, std::string password);
        void signup(std::string username, std::string password);
        bool is_authenticated();

        std::vector<password_t> &list();
        void get(std::size_t id);
        void create(password_t &password);
        void remove(std::size_t id);

    private:
        sqlite3 *db = nullptr;
        struct user_t user;

        void clean_user();
};

#endif // !__PASSWORD_MANAGER__
