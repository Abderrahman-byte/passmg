#include <iostream>
#include <sqlite3.h>
#include <cstdlib>

#include "utils.h"
#include "db.h"

#define DB_PATH "./pass.db"

int main() {
	sqlite3 *db;

	if(!file_exists(DB_PATH)) {
		init_db(DB_PATH, &db);
	} else {
		int rc = sqlite3_open(DB_PATH, &db);
		if(rc != 0 ) {
			std::cerr << "[ERROR] Can't open db" << std::endl;
			exit(1);
		}
	}

	return 0;
}
