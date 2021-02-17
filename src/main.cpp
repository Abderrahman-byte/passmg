#include <iostream>
#include <sqlite3.h>
#include <cstdlib>

#include "utils.h"
#include "db.h"

#define DB_PATH "./pass.db"

int main() {
	sqlite3 *db; 

	/* check if database exists */
	if(!file_exists(DB_PATH)) {
		init_db(DB_PATH, &db); // Initialize database tables
	} else {
		int rc = sqlite3_open(DB_PATH, &db); // open database
		if(rc != 0 ) {
			std::cerr << "[ERROR] Can't open db" << std::endl;
			exit(1);
		}
	}


	/* Login & Register */
	while(true) {
		std::string auth_cmd;

		display_auth_menu();
		std::cout << "ENTER COMMAND >>> ";
		std::cin >> auth_cmd;
		std::cout << std::endl; /**/
		
		if(auth_cmd.length() != 1) {
			std::cerr << "\"" << auth_cmd << "\"" << " is invalid command. try again." << std::endl;
			std::cout << std::endl; /**/
			continue;
		}

		switch(auth_cmd[0]) {
			case '1':
				std::cout << "Login is it" << std::endl;
				break;
			case '2':
				std::cout << "Registration is it" << std::endl;
				break;
			case 'q':
				sqlite3_close(db);
				exit(0);
			default :
				std::cerr << "\"" << auth_cmd << "\"" << " is invalid command. try again." << std::endl;
		}
	}

	return 0;
}
