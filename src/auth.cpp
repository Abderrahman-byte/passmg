#include <iostream>
#include <stdio.h>
#include <sqlite3.h>

#include "utils.h"
#include "models.h"

/* Callback of user select statement*/
int user_selected(void *user, int argc, char **argv, char **azColName) {
	std::cout << "callback " << argc << std::endl;
	for(int i = 0; i < argc; i++) {
		std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL" ) << std::endl;
	}
	return 0;
} /**/

/* Login Prompt */ 
void login(sqlite3 *db) {
	User *user;
	std::string username, password, sql;
	char *zErrMsg = 0;
	
	getchar(); // Flush stdin

	std::cout << "username : " ;
	std::getline(std::cin, username);
	
	echo(false); // Disable terminal echo
	std::cout << "password : " ;
	std::getline(std::cin, password);
	echo(true); // Enable terminal echo	
	std::cout << std::endl;
	
	/*
	sql += "SELECT id,username,password FROM user WHERE username = '" + username + "';";
	int rc = sqlite3_exec(db, sql.c_str(), user_selected, (void *)user, &zErrMsg);
	
	if( rc != SQLITE_OK ) {
		std::cerr <<  "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	} else std::cerr << "Operation done successfully" << std::endl;
	*/
}
