#include <iostream>
#include <string>
#include <sqlite3.h>

int init_db(const std::string name, sqlite3 **db) {
	int rc = sqlite3_open(name.c_str(), db);

	if( rc ) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(*db) << std::endl;
		return -1;	
        }
	
	
	return 0;
}
