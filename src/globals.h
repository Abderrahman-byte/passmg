#include <iostream>

#ifndef _GLOBAL_TYPES_PASS_
#define _GLOBAL_TYPES_PASS_
struct user_data {
	std::string id;
	std::string username;
	std::string password;
};

#define RANDOM_FILE "/dev/random"

/* Ascii characteres to generate random strings */
#define ASCII_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

#endif
