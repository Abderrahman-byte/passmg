#include <iostream>
#include <string>
#include <functional>
#include <sqlite3.h>
#include <fstream>

#include "utils.h"

static int table_created(void *s, int argc, char **argv, char **azColName) { return 0; }

std::string extract_statement(std::ifstream& script) {
	std::string statement;

	while(true) {
		std::string part;
		script >> part;
		statement += part + " ";
		if(part[part.length() - 1] == ';') break;	
	}

	return statement;
}


int init_db(const std::string name, sqlite3 **db) {
	int rc = sqlite3_open(name.c_str(), db); // Open database
	std::ifstream script; // init database sql script
	std::string sql;
	char *zErrMsg;

	/* Check open db error */
	if( rc ) {
		std::cerr << "[ERROR] Can't open database: " << sqlite3_errmsg(*db) << std::endl;
		return -1;	
        }
	
	/* Check if init db script exists */
	if(!file_exists("./pass.sql")) {
		std::cerr << "[ERROR] Cant't find initialization script." << std::endl;
		return -1;
	} 

	script.open("./pass.sql"); // open init sql script
	
	// Extract first sql statement from script stream (CREATE TABLE user ...)
	sql = extract_statement(std::ref(script));
	rc = sqlite3_exec(*db, sql.c_str(), table_created, (void*)NULL, &zErrMsg);

	/* Check exec error */
	if(rc != SQLITE_OK) {
		std::cerr << "[ERROR] SQL error create 'user' : " << zErrMsg << std::endl;
		return -1;
	} else std::cout << "[*] Users table has been created" << std::endl;

	// Extract (CREATE TABLE password ...) sql statement from script stream
	sql = extract_statement(std::ref(script));
	rc = sqlite3_exec(*db, sql.c_str(), table_created, (void*)NULL, &zErrMsg);

	/* Check exec error */
	if(rc != SQLITE_OK) {
		std::cerr << "[ERROR] SQL error create 'password' : " << zErrMsg << std::endl;
		return -1;
	} else std::cout << "[*] Passwords table has been created" << std::endl;
	
	return 0;
}
