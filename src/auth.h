#include <sqlite3.h>
#include "models.h"

#ifndef _AUTH_PASS_H_
#define _AUTH_PASS_H_
extern User login(sqlite3 *);
extern User signup(sqlite3 *);
#endif
