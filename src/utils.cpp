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
