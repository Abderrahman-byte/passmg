#include <string>
#include <fstream>
#include <sqlite3.h>

#ifndef _PASS_DB_H_
#define _PASS_DB_H_

extern int init_db(const std::string, sqlite3**);
extern std::string extract_statement(std::ifstream&);
#endif
