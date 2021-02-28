#include <string>
#include <fstream>
#include <sqlite3.h>

#include "globals.h"

#ifndef _PASS_DB_H_
#define _PASS_DB_H_

extern int init_db(const std::string, sqlite3**);
extern std::string extract_statement(std::ifstream&);
extern user_data get_user_by_username(std::string, sqlite3 *);
int create_user(sqlite3 *, std::string, std::string);
#endif
