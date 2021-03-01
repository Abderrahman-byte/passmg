#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <fstream>

#include "utils.h"
#include "models.h"
#include "crypt.h"

void addPassword(sqlite3 *db, std::ifstream &rndSource, User user) {
	std::string null; // string for flushing data
	std::string title, password, encrypted_password;
	int cipherlen = 1;
	unsigned char *encrypted_data;

	std::getline(std::cin, null); // flush stdin before asking user for input

	std::cout << "Enter password title : ";
	std::cin >> title;	

	std::getline(std::cin, null); // flush stdin before asking user for input
		
	
	std::cout << "Do you want to generate a random password ? [y/n] : ";
	if(getchar() == 'y') password = generateRandomStr(rndSource, 20);
	else {
		std::getline(std::cin, null); // flush stdin before asking user for input
		std::cout << "Enter password : ";
		std::cin >> password;
	}

	encrypted_data = encrypt_aes_256(password, user.get_password(), &cipherlen);
	encrypted_password = to_hex(encrypted_data, cipherlen);

	std::cout << "Title : " << title << ", Password : " << password << std::endl;
	std::cout << "Encrypted : " << encrypted_password << std::endl;	

}
