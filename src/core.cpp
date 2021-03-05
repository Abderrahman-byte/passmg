#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <fstream>

#include "utils.h"
#include "models.h"
#include "db.h"
#include "crypt.h"

void addPassword(sqlite3 *db, std::ifstream &rndSource, User user) {
	std::string null; // string for flushing data
	std::string title, password, encrypted_password;
	int cipherlen = 1;
	unsigned char *encrypted_data;
	
	do {
		std::getline(std::cin, null); // flush stdin before asking user for input	

		std::cout << "Enter password title : ";
		std::cin >> title;
		
		if(password_with_title_exists(db, title, user.get_username())) {
			std::cout << "[FAILED] Password with title \"" << title << "\" already exists. try again";
			std::cout << std::endl << std::endl;
			title = "";
		}
	} while(title.compare("") == 0);

	std::getline(std::cin, null); // flush stdin before asking user for input
		
	std::cout << "Do you want to generate a random password ? [y/n] : ";
	if(getchar() == 'y') password = generateRandomStr(rndSource, 20);
	else {
		std::getline(std::cin, null); // flush stdin before asking user for input
		std::cout << "Enter password : ";
		std::cin >> password;
	}


	encrypted_data = encrypt_aes_256(password , user.get_password(), &cipherlen);
	encrypted_password = to_hex(encrypted_data, cipherlen);

	std::cout << "\ttitle : " << title << std::endl ;
	std::cout << "\tpassword : " << password << std::endl ;

	if(insert_password(db, get_user_id(db, user.get_username()), title, encrypted_password) == 0) 
		std::cout << "[SUCCESS] Password added safely" << std::endl;
	else 
		std::cout << "[FAILED] Couldnt save new password into database" << std::endl;
}

void getPasswordsList(sqlite3 *db, User user) {
	std::vector<std::string> pws_list = get_user_passwords_list(db, get_user_id(db, user.get_username()));
	
	for(int i = 0; i < pws_list.size(); i++) {
		std::cout << "\t[*] " << pws_list[i] << std::endl;
	}
}
