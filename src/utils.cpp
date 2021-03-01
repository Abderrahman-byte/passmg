#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <sys/stat.h>
#include <termios.h>

#include "globals.h"

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

unsigned int randomRange(std::ifstream &source, unsigned int max, unsigned int min = 0) {
	unsigned char randomBuffer[10];
	unsigned int sum, rnd;

	source.read((char *)randomBuffer, 10);
	for(int i = 0; i < 10; i++) sum += (int)randomBuffer[i];
	rnd = (sum % (max - min + 1)) + min;

	return rnd;
}

std::string generateRandomStr(std::ifstream &source, unsigned int len) {
	std::string allowed = ASCII_CHARS;
	std::string rnd_str;

	if(!source.is_open()) source.open(RANDOM_FILE); // Opening file lazily

	for(int i = 0; i < len; i++) {
		int index = randomRange(source, allowed.length(), 0);
		rnd_str += allowed[index];
	}
	
	return rnd_str;
}


void closeIfOpen(std::ifstream &file) {
	if(file.is_open()) file.close(); // Check if file is open then close it
}

std::string to_hex(unsigned char *data, int len) {
	std::stringstream ss;

	for(int i = 0 ; i < len; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] ;
	}

	return ss.str();
}

int parseHex(char digit) {
	unsigned char n;

	switch(digit) {
		case '0': n = 0; break; 
		case '1': n = 1; break; 
		case '2': n = 2; break; 
		case '3': n = 3; break; 
		case '4': n = 4; break; 
		case '5': n = 5; break; 
		case '6': n = 6; break; 
		case '7': n = 7; break; 
		case '8': n = 8; break; 
		case '9': n = 9; break; 
		case 'a': n = 10; break; 
		case 'b': n = 11; break; 
		case 'c': n = 12; break; 
		case 'd': n = 13; break; 
		case 'e': n = 14; break; 
		case 'f': n = 15; break; 
	}
	
	return n;
}

unsigned char* from_hex(std::string hex_data) {
	unsigned char* data;
	data = (unsigned char *)malloc((hex_data.length() / 2) * sizeof(unsigned char));

	for(int i = 0; i < hex_data.length() - 1; i += 2) {
		unsigned char byte;
		byte = parseHex(hex_data[i]);
		byte = byte << 4;
		byte += parseHex(hex_data[i + 1]);
		data[i / 2] = byte;
	}
	
	return data;
}
