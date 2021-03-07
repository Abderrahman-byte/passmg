#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <fstream>
#include <openssl/sha.h>

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

User ChangeMasterPassword(sqlite3 *db, User user) {
	std::string null, old_password, new_password, re_new_password, user_id, hashed_password;
	std::vector<std::string> pws_list;

	std::getline(std::cin, null); // Flush

	echo(false) ; // disable echo
	std::cout << "Enter your current password : " ;
	std::cin >> old_password ;
	std::cout << std::endl;
	echo(true); // re-enable echo

	if(old_password.compare(user.get_password()) != 0) {
		std::cerr << "[FAILED] current password is wrong" << std::endl;
		return user;
	}

	echo(false) ;
	std::cout << "Enter your new password : " ;
	std::cin >> new_password;
	std::cout << std::endl;

	std::cout << "Confirm your new password : ";
	std::cin >> re_new_password;
	std::cout << std::endl;
	echo(true);

	if(new_password.compare(re_new_password) != 0) {
		std::cerr << "[FAILED] New passwords doesn't match." << std::endl;
		return user;
	} 

	if(new_password.compare(old_password) == 0) {
		std::cerr << "[!] New password is the old password" << std::endl;
		return user;	
	}
	
	user_id = get_user_id(db, user.get_username());
	pws_list = get_user_passwords_list(db, user_id);

	for(int i = 0; i < pws_list.size(); i++) {
		unsigned char *encrypted_content, *new_encrypted_password;
		int new_cipherlen;
		std::string encrypted_content_hex, new_encrypted_password_hex, content;

		encrypted_content_hex= get_password_content(db, user_id, pws_list[i]);
		encrypted_content = from_hex(encrypted_content_hex);
		content = decrypt_aes_256(encrypted_content, encrypted_content_hex.length() / 2, user.get_password());
		
		new_encrypted_password = encrypt_aes_256(content , new_password, &new_cipherlen);
		new_encrypted_password_hex = to_hex(new_encrypted_password, new_cipherlen);

		update_password_content(db, user_id, pws_list[i], new_encrypted_password_hex);
	}

	hashed_password = to_hex(sha256(new_password), SHA256_DIGEST_LENGTH);
	update_user_data(db, user_id, user.get_username(), hashed_password);
	user = User(user.get_username(), new_password, hashed_password);

	return user;
}

void DeleteAccount(sqlite3 *db, User &user) {
	std::string user_id = get_user_id(db, user.get_username());
	std::vector<std::string> pws_list = get_user_passwords_list(db, user_id);

	for(int i = 0; i < pws_list.size(); i++)
		delete_password(db, user.get_username(), pws_list[i]);

	std::cout << "[*] User passwords has been deleted." << std::endl;
	
	delete_user(db, user_id);
	std::cout << "[*] User account has been deleted." << std::endl;
	user.logout();
}
