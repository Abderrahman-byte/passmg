#include <iostream>
#include <string>
#include <sys/stat.h>

bool file_exists(const std::string& p) {
	struct stat info;
	return (stat(p.c_str(), &info) == 0) ;
}

void display_auth_menu() {
	std::cout << "Authentication :" << std::endl;
	std::cout << "[1] Login" << std::endl;
	std::cout << "[2] Register" << std::endl;
	std::cout << "[q] quit" << std::endl;
}
