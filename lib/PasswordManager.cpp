#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

#include <sqlite3.h>

#include "db.cpp"
#include "libpassmg/PasswordManager.hpp"
#include "libpassmg/config.hpp"
#include "libpassmg/crypt.hpp"
#include "libpassmg/exceptions.hpp"

/* a very basic intput checking */
void check_user_login_input(std::string username, std::string password) {
    if (username.length() < 4) {
        throw InvalidInputException("Invalid intput : username is too short");
    }

    if (password.length() < 6) {
        throw InvalidInputException(
            "Invalid intput : password must be at least 6 chars long");
    }
}

PasswordManager::PasswordManager(std::string dbname) {
    char *zErrMsg = nullptr;
    int rc = sqlite3_open(dbname.c_str(), &db);

    if (rc != SQLITE_OK) {
        throw DatabaseException("Cannot opent database file");
    }

    rc = init_tables(db, &zErrMsg);

    clean_user();

    if (rc != SQLITE_OK) {
        // FIXME throwing Exception before freeing zErrMsg causes memory leak
        throw DatabaseException(zErrMsg);
    }

    if (zErrMsg != nullptr) free(zErrMsg);
}

PasswordManager::~PasswordManager() {
    //
    sqlite3_close(db);
}

void PasswordManager::signup(std::string username, std::string password) {
    char *zErrMsg = nullptr;
    int rc;

    check_user_login_input(username, password);

    user.id = 0;
    user.username = username;
    user.pw = password;
    user.hashed_pw = sha256_sum(password);

    rc = create_user(db, &zErrMsg, user);

    if (rc == SQLITE_OK) {
        //
    } else if (rc == SQLITE_CONSTRAINT) {
        throw IntegrityException("user with the same username already exist");
    } else {
        throw DatabaseException(zErrMsg);
    }

    if (zErrMsg != nullptr) free(zErrMsg);
}

void PasswordManager::login(std::string username, std::string password) {
    int rc;

    check_user_login_input(username, password);

    user.id = 0;
    user.username = username;

    rc = select_user_by_username(db, user);

    if (rc != SQLITE_OK) throw DatabaseException();

    if (user.id == 0) throw WrongCredentialsException();

    if (sha256_sum(password).compare(user.hashed_pw) != 0) {
        clean_user();
        throw WrongCredentialsException();
    }

    user.pw = password;
}

bool PasswordManager::is_authenticated() {
    return user.id > 0 && user.username.length() > 0;
}

void PasswordManager::clean_user() {
    user.id = 0;
    user.username = "";
    user.hashed_pw = "<unmatchable>";
    user.pw = "";
}
