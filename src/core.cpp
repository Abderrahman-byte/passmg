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
	
	if(pws_list.size() == 0) {
		std::cout << "\t[!] No passwords has been saved in your account." << std::endl;
		return ;
	}

	for(int i = 0; i < pws_list.size(); i++) {
		std::cout << "\t[*] " << pws_list[i] << std::endl;
	}
}

void getUserPassword(sqlite3 *db, User user) {
	unsigned char* encrypted_data;
	std::string encrypted_password, title, password;
	
	std::cout << "Enter password title : ";
	std::cin >> title;

	if(!password_with_title_exists(db, title, user.get_username())) {
		std::cout << "[FAILED] Password with title \"" << title << "\" doesn\'t exists" << std::endl;
		return ;
	}
		
	encrypted_password = get_password_content(db, get_user_id(db, user.get_username()), title);

	if (encrypted_password.compare("") == 0) {
		std::cerr << "[FAILED] Couldn't get password with title " << title << std::endl;
		return;
	}

	encrypted_data = from_hex(encrypted_password);
	password = decrypt_aes_256(encrypted_data, encrypted_password.length() / 2, user.get_password());
	
	std::cout << "\ttitle : " << title << std::endl;
	std::cout << "\tpassword : " << password << std::endl;
}

void DropPassword(sqlite3 *db, User user) {
	std::string title, null;
	
	do {
		std::getline(std::cin, null); // flush stdin before asking user for input	
		std::cout << "Enter title of the password to be deleted : ";
		std::cin >> title;

	} while (title.compare("") == 0);

	if(!password_with_title_exists(db, title, user.get_username())) {
		std::cerr << "[FAILED] Password with title \'" << title << "\' doesn\'t exists." << std::endl;
		return;
	}
	
	std::getline(std::cin, null); // flush stdin before asking user for input	
	std::cout << "Do you really want to delete \'" << title << "\' password ? [y/n] : ";

	if(getchar() != 'y') return ;

	if(delete_password(db, user.get_username(), title) == 0)
		std::cout << "[SUCCESS] Password \'" << title << "\' has been deleted" << std::endl;	
	else
		std::cout << "[FAILED] Couldn't delete password" << std::endl;
}

User ChangeUsername(sqlite3 *db, User user) {
	std::string new_username, null;
	int rc;

	do {
		std::getline(std::cin, null);
		std::cout << "Enter new username : " ;
		std::cin >> new_username;

		if(get_user_by_username(new_username, db).id.compare("") != 0) {
			std::cout << "[FAILED] Username \'" << new_username  << "\' already used. try again" << std::endl;
			new_username = "";
		} else if(new_username.length() < 6) {
			std::cout << "[FAILED] Username is too short. try again" << std::endl;	
			new_username = "";
		}

	} while(new_username.compare("") == 0);

	rc = update_user_data(db, get_user_id(db, user.get_username()), new_username, user.get_hashed_password());
	if(rc == 0) {
		user = User(new_username, user.get_password(), user.get_hashed_password());
		std::cout << "[SUCCESS] username has been changed to \'" << new_username << "\'." << std::endl;
	}
		
	return user;
}
