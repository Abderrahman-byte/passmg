#include <sqlite3.h>
#include <fstream>

#include "models.h"

#ifndef _PASS_CORE_H_
#define _PASS_CORE_H_

extern void addPassword(sqlite3 *, std::ifstream &, User);

#endif
