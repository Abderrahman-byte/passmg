#include <sqlite3.h>
#include <fstream>

#include "models.h"

#ifndef _PASS_CORE_H_
#define _PASS_CORE_H_

extern void addPassword(sqlite3 *, std::ifstream &, User);
extern void getPasswordsList(sqlite3 *, User);
extern void getUserPassword(sqlite3 *, User);
extern void DropPassword(sqlite3 *, User);
extern User ChangeUsername(sqlite3 *, User);
extern User ChangeMasterPassword(sqlite3 *, User);
extern void DeleteAccount(sqlite3 *, User &);
#endif
