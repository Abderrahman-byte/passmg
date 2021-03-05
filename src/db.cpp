#include <iostream>
#include <string>
#include <functional>
#include <sqlite3.h>
#include <fstream>
#include <cstring>
#include <vector>

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

/* Callback when user id is selected */
int user_id_selected(void *id, int argc, char **argv, char **azColName) {
	std::string *id_data = (std::string *)id;

	for(int i = 0; i < argc; i++) {
		std::string col = convertToString(azColName[i], strlen(azColName[i]));
		std::string value = convertToString(argv[i], strlen(argv[i]));

		if(col.compare("id") == 0) *id_data = value;
	}

	return 0;
}

/* Callback if password title exists */
int password_title_selected(void *exists, int argc, char **argv, char **azColName) {
	bool *exists_ptr = (bool *)exists;
	*exists_ptr = true;

	return 0;
}

/* Callback when passwords list selected */
int passwords_list_selected(void *list, int argc, char **argv, char **azColName) {
	std::vector<std::string> *pw_list = (std::vector<std::string> *)list;
	
	for(int i = 0; i < argc; i++) {
		std::string col = convertToString(azColName[i], strlen(azColName[i]));
		std::string value = convertToString(argv[i], strlen(argv[i]));

		if(col.compare("title") == 0) pw_list->push_back(value);
	}
	
	return 0;
}

/* Callback when password content selected */
int password_content_selected(void *content, int argc, char **argv, char **azColName) {
	std::string *output = (std::string *)content;

	for(int i = 0; i < argc; i++) {	
		std::string col = convertToString(azColName[i], strlen(azColName[i]));
		std::string value = convertToString(argv[i], strlen(argv[i]));

		if(col.compare("content") == 0) *output = value;
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

/* get id of a user */
std::string get_user_id(sqlite3 *db, std::string username) {
	std::string id = "";
	std::string sql = "SELECT id FROM user WHERE username = '" + username + "';";

	int rc;
	char *ErrMsg;

	rc = sqlite3_exec(db, sql.c_str(), user_id_selected, (void *)&id, &ErrMsg);

	if(rc != SQLITE_OK) {
		std::cerr << "[SQL ERROR] " << ErrMsg << std::endl ;
	       	return "";
	}

	return id;
}

/* Check if pasword title exists */
bool password_with_title_exists(sqlite3 *db, std::string title, std::string username) {
	bool exists = false;
	std::string user_id = get_user_id(db, username), sql ;
	int rc;
	char *ErrMsg;

	if(user_id.compare("") == 0) return false;	

	sql = "SELECT id FROM password WHERE title = '" + title + "' AND user_id = " + user_id + ";";
	rc = sqlite3_exec(db, sql.c_str(), password_title_selected, (void *)&exists, &ErrMsg);

	if(rc != SQLITE_OK) {
		std::cerr << "[SQL ERROR] " << ErrMsg << std::endl;
		return false;
	}

	return exists;
}

int insert_password(sqlite3 *db, std::string user_id, std::string title, std::string content) {
	std::string sql;
	int rc;
	char *ErrMsg;

	sql = "INSERT INTO password (user_id, title, content) VALUES (" + user_id + ", '" + title + "', '" + content + "');";
	rc = sqlite3_exec(db, sql.c_str(), do_nothing, NULL, &ErrMsg);

	if(rc != SQLITE_OK) {
		std::cerr << "[SQL ERROR] " << ErrMsg << std::endl;
		return -1;
	}

	return 0;
}

std::vector<std::string> get_user_passwords_list(sqlite3 *db, std::string user_id) {
	std::vector<std::string> passwords_list;
	std::string sql;
	int rc;
	char *ErrMsg;
	
	sql = "SELECT title FROM password WHERE user_id = " + user_id + ";";
	rc = sqlite3_exec(db, sql.c_str(), passwords_list_selected, (void *)&passwords_list, &ErrMsg);
	
	if(rc != SQLITE_OK) {
		std::cerr << "[SQL ERROR] " << ErrMsg << std::endl;
	}	

	return passwords_list;
}

std::string get_password_content(sqlite3 *db, std::string user_id, std::string title) {
	std::string content;
	std::string sql;
	int rc;
	char *ErrMsg;
	
	sql = "SELECT content FROM password WHERE user_id = " + user_id + " AND title = '" + title + "' ;";
	rc = sqlite3_exec(db, sql.c_str(), password_content_selected, (void *)&content, &ErrMsg);

	if(rc != SQLITE_OK) {
		std::cerr << "[SQL ERROR] " << ErrMsg << std::endl;
		return "";
	}

	return content;
}
