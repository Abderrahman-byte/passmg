#include <sqlite3.h>

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "auth.h"
#include "core.h"
#include "db.h"
#include "globals.h"
#include "models.h"
#include "utils.h"

int main() {
    std::cout << "hello from main" << std::endl;

    std::cout << USER_TABLE_DDL << std::endl;
    std::cout << PASSWORD_TABLE_DDL << std::endl;

    return 0;
}
