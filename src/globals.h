#include <iostream>

#ifndef _GLOBAL_TYPES_PASS_
#define _GLOBAL_TYPES_PASS_

#define RANDOM_FILE "/dev/random"
// #define DB_PATH "~/.local/share/pass/pass.db"
// #define INIT_SCRIPT "~/.local/share/pass/pass.sql"

/* Ascii characteres to generate random strings */
#define ASCII_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

struct user_data {
	std::string id;
	std::string username;
	std::string password;
};

#endif
