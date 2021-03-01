#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <fstream>

#include "utils.h"
#include "models.h"

void addPassword(sqlite3 *db, std::ifstream &rndSource, User user) {
	std::string null; // string for flushing data
	std::string title;
	std::string password;

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

	std::cout << "Title : " << title << ", Password : " << password << std::endl;
	
}
