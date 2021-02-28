#include <iostream>
#include <string>
#include <functional>
#include <sqlite3.h>
#include <fstream>
#include <cstring>

#include "globals.h"
#include "utils.h"

/* Callback That does nothing */
int do_nothing(void *s, int argc, char **argv, char **azColName) { return 0; }

/* Callback when user data row is returned from select query */
int user_selected(void *data, int argc, char **argv, char **azColName) {
	struct user_data *user = (struct user_data *)data;

	for(int i = 0; i < argc; i++) {
		std::string col = convertToString(azColName[i], strlen(azColName[i]));
		std::string value = convertToString(argv[i], strlen(argv[i]));
		
		if(col.compare("username") == 0) user->username = argv[i] ? value : "";
		else if(col.compare("password") == 0) user->password = argv[i] ? value : "";
		else if(col.compare("id") == 0) user->id = argv[i] ? value : "";
	}

	return 0;
}

/* Extract on sql statement from sql script stream*/
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

/* Get user data by username */
struct user_data get_user_by_username(std::string username, sqlite3 *db) {
	int rc;
	struct user_data user;
	char *ErrMsg;
	std::string query = "SELECT id, username, password FROM user WHERE username = '" + username + "';";	
	
	rc = sqlite3_exec(db, query.c_str(), user_selected, (void *)&user, &ErrMsg);

	if(rc != 0) {
		std::cerr << "SQL Error : " << ErrMsg << std::endl;
		throw ErrMsg;
	}

	return user;
}

/* Initialize database tables */
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
	rc = sqlite3_exec(*db, sql.c_str(), do_nothing, (void*)NULL, &zErrMsg);

	/* Check exec error */
	if(rc != SQLITE_OK) {
		std::cerr << "[ERROR] SQL error create 'user' : " << zErrMsg << std::endl;
		return -1;
	} else std::cout << "[*] Users table has been created" << std::endl;

	// Extract (CREATE TABLE password ...) sql statement from script stream
	sql = extract_statement(std::ref(script));
	rc = sqlite3_exec(*db, sql.c_str(), do_nothing, (void*)NULL, &zErrMsg);

	/* Check exec error */
	if(rc != SQLITE_OK) {
		std::cerr << "[ERROR] SQL error create 'password' : " << zErrMsg << std::endl;
		return -1;
	} else std::cout << "[*] Passwords table has been created" << std::endl;
	
	return 0;
}

/* Insert new user into database */
int create_user(sqlite3 *db, std::string username, std::string hashed_password) {
	std::string sql = "INSERT INTO user (username, password) VALUES ('" + username + "', '" + hashed_password + "') ;";
	int rc ;
	char *ErrMsg;

	rc = sqlite3_exec(db, sql.c_str(), do_nothing, (void *)NULL, &ErrMsg);

	if(rc != SQLITE_OK) {
		std::cerr << "[SQL ERROR] " << ErrMsg << std::endl ;
		return -1;
	}

	return 0;
}
