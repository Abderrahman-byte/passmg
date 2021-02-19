#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/stat.h>
#include <termios.h>

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

void display_actions_menu() {
	// passwords CRUD operations
	std::cout << "[1] List your passwords" << std::endl;
	std::cout << "[2] Get a password" << std::endl;
	std::cout << "[3] Generate new password" << std::endl;
	std::cout << "[4] Delete password" << std::endl;
	
	// Account changes
	std::cout << "[5] Change your username" << std::endl;
	std::cout << "[6] Change master password" << std::endl;
	std::cout << "[7] delete your account" << std::endl;

	std::cout << "[8] logout" << std::endl;
	std::cout << "[q] quit" << std::endl;
}

void echo(bool status=true) {
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	
	if(status) t.c_lflag |= ECHO;
	else t.c_lflag &= ~ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

std::string convertToString(char *a, int size) {
	std::string output;

	for(int i = 0; i < size; i++) {
		output += a[i];
	}

	return output;
} 
