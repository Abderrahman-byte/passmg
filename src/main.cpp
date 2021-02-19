#include <iostream>
#include <sqlite3.h>
#include <cstdlib>

#include "utils.h"
#include "db.h"
#include "auth.h"
#include "models.h"

#define DB_PATH "./pass.db"

int main() {
	sqlite3 *db; 

	/* check if database exists */
	if(!file_exists(DB_PATH)) {
		if(init_db(DB_PATH, &db) != 0) return -1 ; // Initialize database tables
	} else {
		int rc = sqlite3_open(DB_PATH, &db); // open database
		if(rc != 0 ) {
			std::cerr << "[ERROR] Can't open db" << std::endl;
			exit(1);
		}
	}


	/* Login & Register */
	while(true) {
		User user;
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
				user = login(db);
				break;
			case '2':
				user = signup(db);
				break;
			case 'q':
				sqlite3_close(db);
				exit(0);
			default :
				std::cerr << "\"" << auth_cmd << "\"" << " is invalid command. try again." << std::endl;
		}
		
		std::cout << std::endl;
		if(user.is_anonymous()) continue; // On login/register failed

		while(!user.is_anonymous()) {
			std::string action_cmd;
			
			display_actions_menu();
			std::cout << std::endl << user.get_username() << " > ";
			std::cin >> action_cmd;

			if(action_cmd.length() != 1) {
				std::cerr << "\"" << action_cmd << "\"" << " is invalid command. try again." << std::endl;
				std::cout << std::endl ; /**/
				continue;
			}

			switch(action_cmd[0]) {
				case '8' :
					user.logout();
					std::cout << "[SUCCESS] logged out" << std::endl;
					break;

				case 'q' :
					sqlite3_close(db);
					exit(0);

				default :
					std::cerr << "\"" << action_cmd << "\"" << " is invalid command. try again." << std::endl;
			}

			std::cout << std::endl;
		}
	}

	return 0;
}
