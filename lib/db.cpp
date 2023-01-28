#include <cstdlib>
#include <cstring>
#include <string>

#include <sqlite3.h>

#include "libpassmg/config.hpp"
#include "libpassmg/types.hpp"

#ifndef _PASSMG_DB__
#define _PASSMG_DB__

/* Callback That does nothing */
int do_nothing(void *s, int argc, char **argv, char **azColName) { return 0; }

int set_user_data(void *s, int argc, char **argv, char **azColName) {
    // TODO replace c style casting with something more efficient
    struct user_t *user_data = (struct user_t *)s;

    for (int i = 0; i < argc; i++) {
        std::string colname(azColName[i], strlen(azColName[i]));

        if (colname.compare("id") == 0) {
            // FIXME this is not safe and it may also throw an error if the
            // value in argv in not an int
            user_data->id = std::stoi(argv[i]);
        }

        if (colname.compare("password") == 0) {
            user_data->hashed_pw = std::string(argv[i], strlen(argv[i]));
        }

        if (colname.compare("username") == 0) {
            user_data->username = std::string(argv[i], strlen(argv[i]));
        }
    }

    return 0;
}

int init_tables(sqlite3 *db, char **zErrMsg) {
    int rc = sqlite3_exec(db, USER_TABLE_DDL, do_nothing, 0, zErrMsg);

    if (rc != SQLITE_OK) return rc;

    rc = sqlite3_exec(db, PASSWORD_TABLE_DDL, do_nothing, 0, zErrMsg);

    return rc;
}

int create_user(sqlite3 *db, char **zErrMsg, struct user_t &data) {
    // TODO use sqlite3_prepare instead
    std::string query = "INSERT Into user (username,password) values "
                        "('" +
                        data.username + "','" + data.hashed_pw +
                        "') returning id;";

    int rc =
        sqlite3_exec(db, query.c_str(), set_user_data, (void *)&data, zErrMsg);

    return rc;
}

int select_user_by_username(sqlite3 *db, struct user_t &data) {
    std::string query =
        "SELECT username, password, id FROM user WHERE username = '" +
        data.username + "';";

    int rc =
        sqlite3_exec(db, query.c_str(), set_user_data, (void *)&data, NULL);

    return rc;
}

#endif // !_passmg_db__
