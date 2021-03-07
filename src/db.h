#include <string>
#include <fstream>
#include <sqlite3.h>
#include <vector>

#include "globals.h"

#ifndef _PASS_DB_H_
#define _PASS_DB_H_

extern int init_db(const std::string, sqlite3**);
extern std::string extract_statement(std::ifstream&);
extern user_data get_user_by_username(std::string, sqlite3 *);
extern int create_user(sqlite3 *, std::string, std::string);
extern std::string get_user_id(sqlite3 *, std::string);
extern bool password_with_title_exists(sqlite3 *, std::string, std::string);
extern int insert_password(sqlite3 *, std::string, std::string, std::string);
extern std::vector<std::string> get_user_passwords_list(sqlite3 *, std::string);
extern std::string get_password_content(sqlite3 *, std::string, std::string);
extern int delete_password(sqlite3 *, std::string, std::string);
extern int update_user_data(sqlite3 *, std::string, std::string, std::string);
#endif
