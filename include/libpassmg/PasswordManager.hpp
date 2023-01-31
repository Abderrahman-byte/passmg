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

        std::vector<struct password_t> list();
        struct password_t get(std::size_t id);
        struct password_t get(std::string title);
        struct password_t create(std::string title, std::string content);
        void remove(std::size_t id);
        void remove(std::string title);
        std::string get_username();

    private:
        sqlite3 *db = nullptr;
        struct user_t user = {0};
};

#endif // !__PASSWORD_MANAGER__
