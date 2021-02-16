#include <iostream>
#include <sqlite3.h>
#include "utils.h"
#include "db.h"

#define DB_PATH "./pass.db"

int main() {
	sqlite3 *db;
	std::cout << "C++ " << __cplusplus << std::endl;

	if(!file_exists(DB_PATH)) {
		std::cout << "First Time" << std::endl;
		init_db(DB_PATH, &db);
	}

	return 0;
}
