#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>

#include <sqlite3.h>
#include <vector>

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
        user = {0};
        throw WrongCredentialsException();
    }

    user.pw = password;
}

struct password_t PasswordManager::create(std::string title,
                                          std::string content) {
    if (!is_authenticated()) throw AuthenticationRequired();

    struct password_t password = {0};
    int rc;

    if (title.length() < 4)
        throw InvalidInputException("Password title is toot short");

    password.title = title;
    password.content = content;
    password.cipher_content = encrypt_aes_256_hex(content, user.pw);

    rc = create_password(db, user.id, password);

    if (rc == SQLITE_CONSTRAINT) {
        throw IntegrityException("Password with same title already exists.");
    } else if (rc != SQLITE_OK) {
        throw DatabaseException();
    }

    return password;
}

struct password_t PasswordManager::get(std::size_t id) {
    if (!is_authenticated()) throw AuthenticationRequired();

    struct password_t password = {0};
    int rc = select_password(db, user.id, "id", std::to_string(id), password);

    if (rc != SQLITE_OK) throw DatabaseException();

    if (password.id > 0 && password.cipher_content.length() > 0) {
        password.content =
            decrypt_aes_256_hex(password.cipher_content, user.pw);
    }

    // maybe throw exception if password doesn't exist

    return password;
}

struct password_t PasswordManager::get(std::string title) {
    if (!is_authenticated()) throw AuthenticationRequired();

    struct password_t password = {0};
    int rc = select_password(db, user.id, "title", title, password);

    if (rc != SQLITE_OK) throw DatabaseException();

    if (password.id > 0 && password.cipher_content.length() > 0) {
        password.content =
            decrypt_aes_256_hex(password.cipher_content, user.pw);
    }

    // maybe throw exception if password doesn't exist

    return password;
}

std::vector<struct password_t> PasswordManager::list(bool w_content) {
    if (!is_authenticated()) throw AuthenticationRequired();

    std::vector<struct password_t> passwords;

    int rc = select_user_passwords(db, user.id, passwords, w_content);

    if (rc != SQLITE_OK) throw DatabaseException();

    for (auto &pw : passwords) {
        if (pw.cipher_content.length() > 0)
            pw.content = decrypt_aes_256_hex(pw.cipher_content, user.pw);
    }

    return passwords;
}

void PasswordManager::remove(std::size_t id) {
    if (!is_authenticated()) throw AuthenticationRequired();

    int rc = delete_password(db, user.id, "id", std::to_string(id));

    if (rc != SQLITE_OK) throw DatabaseException();
}

void PasswordManager::remove(std::string title) {
    if (!is_authenticated()) throw AuthenticationRequired();

    int rc = delete_password(db, user.id, "title", title);

    if (rc != SQLITE_OK) throw DatabaseException();
}

bool PasswordManager::is_authenticated() {
    return user.id > 0 && user.username.length() > 0;
}

std::string PasswordManager::get_username() {
    if (!is_authenticated()) throw AuthenticationRequired();

    return user.username;
}
