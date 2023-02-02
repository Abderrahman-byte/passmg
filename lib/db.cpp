#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "libpassmg/config.hpp"
#include "libpassmg/types.hpp"

#ifndef _PASSMG_DB__
#define _PASSMG_DB__

/* Callback That does nothing */
int do_nothing(void *s, int argc, char **argv, char **azColName) { return 0; }

/* A callback for select row from user table */
int set_user_data(void *s, int argc, char **argv, char **azColName) {
    // TODO replace c style casting with something more efficient
    struct user_t *user_data = static_cast<struct user_t *>(s);

    for (int i = 0; i < argc; i++) {
        std::string colname(azColName[i], strlen(azColName[i]));

        if (colname.compare("id") == 0) {
            // FIXME this is not safe and it may also throw an error if the
            // value in argv in not an int
            user_data->id = std::stoul(argv[i]);
        } else if (colname.compare("password") == 0) {
            user_data->hashed_pw = std::string(argv[i], strlen(argv[i]));
        } else if (colname.compare("username") == 0) {
            user_data->username = std::string(argv[i], strlen(argv[i]));
        }
    }

    return 0;
}

int set_password_data(void *s, int argc, char **argv, char **azColName) {
    struct password_t *password = static_cast<struct password_t *>(s);

    for (std::size_t i = 0; i < argc; i++) {
        std::string colname = std::string(azColName[i], strlen(azColName[i]));

        if (colname.compare("id") == 0) {
            password->id = std::stoul(argv[i]);
        } else if (colname.compare("title") == 0) {
            password->title = std::string(argv[i], strlen(argv[i]));
        } else if (colname.compare("content") == 0) {
            password->cipher_content = std::string(argv[i], strlen(argv[i]));
        }
    }

    return 0;
}

int add_password_data(void *s, int argc, char **argv, char **azColName) {
    struct password_t password = {0};
    std::vector<struct password_t> *pws =
        static_cast<std::vector<struct password_t> *>(s);

    set_password_data(static_cast<void *>(&password), argc, argv, azColName);

    pws->push_back(password);

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

    int rc = sqlite3_exec(db, query.c_str(), set_user_data,
                          static_cast<void *>(&data), zErrMsg);

    return rc;
}

int select_user_by_username(sqlite3 *db, struct user_t &data) {
    std::string query =
        "SELECT username, password, id FROM user WHERE username = '" +
        data.username + "';";

    int rc = sqlite3_exec(db, query.c_str(), set_user_data,
                          static_cast<void *>(&data), NULL);

    return rc;
}

int create_password(sqlite3 *db, std::size_t user_id,
                    struct password_t &password) {
    std::string query =
        "INSERT INTO password (user_id, title, content) VALUES ('" +
        std::to_string(user_id) + "', '" + password.title + "', '" +
        password.cipher_content + "') RETURNING id;";

    int rc = sqlite3_exec(db, query.c_str(), set_password_data,
                          static_cast<void *>(&password), NULL);

    return rc;
}

int select_password(sqlite3 *db, std::size_t user_id, std::string key,
                    std::string value, struct password_t &pw) {
    std::string query = "SELECT id, title, content FROM password WHERE " + key +
                        " =  '" + value +
                        "' AND user_id = " + std::to_string(user_id) + ";";

    int rc = sqlite3_exec(db, query.c_str(), set_password_data,
                          static_cast<void *>(&pw), NULL);

    return rc;
}

int select_user_passwords(sqlite3 *db, std::size_t user_id,
                          std::vector<struct password_t> &passwords,
                          bool with_content = false) {
    std::string query = "SELECT id, title";

    if (with_content) query += ", content";

    query += "  FROM password WHERE user_id='" + std::to_string(user_id) + "';";

    int rc =
        sqlite3_exec(db, query.c_str(), add_password_data, &passwords, NULL);

    return rc;
}

int delete_password(sqlite3 *db, std::size_t user_id, std::string key,
                    std::string value) {
    std::string query = "DELETE FROM password WHERE " + key + " =  '" + value +
                        "' AND user_id = " + std::to_string(user_id) + ";";

    int rc = sqlite3_exec(db, query.c_str(), do_nothing, NULL, NULL);

    return rc;
}

#endif // !_passmg_db__
