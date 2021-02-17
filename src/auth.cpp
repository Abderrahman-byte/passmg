#include <iostream>
#include <stdio.h>

#include "utils.h"

/* Login Prompt */ 
void login() {
	std::string username, password;
	
	getchar(); // Flush stdin

	std::cout << "username : " ;
	std::getline(std::cin, username);
	
	echo(false); // Disable terminal echo
	std::cout << "password : " ;
	std::getline(std::cin, password);
	echo(true); // Enable terminal echo
	

}
