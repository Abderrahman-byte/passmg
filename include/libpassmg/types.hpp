#include <cstddef>
#include <string>

#ifndef _PASSMG_TYPES_
#define _PASSMG_TYPES_

struct password_t {
        std::size_t id;
        std::string title;
        std::string content;
        std::string cipher_content;
};

struct user_t {
        std::size_t id;
        std::string username;
        std::string pw;
        std::string hashed_pw;
};

#endif // !_PASSMG_TYPES_
