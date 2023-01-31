#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <termios.h>

#include "libpassmg/config.hpp"
#include "utils.hpp"

// Get version text
std::string get_version() {
    std::string version = std::string(PROJECT_NAME) +
                          " version : " + PASSMG_VERSION +
                          "\nRelease : " + __DATE__;
    return version;
}

/* a safer version of getenv, it returns empty string if the getenv returns a
 * null pointer  */
std::string passmg_getenv(const char *env) {
    char *value = std::getenv(env);

    if (value == nullptr) return "";

    return std::string(value);
}

void mkpath(std::string path) {
    std::string current;

    if (path.empty()) throw std::invalid_argument("path cannot be empty");

    for (size_t pos = 0; (pos = path.find('/')) != std::string::npos;) {
        current += path.substr(0, pos + 1);

        do_mkdir(current);

        path.erase(0, pos + 1);
    }

    if (!path.empty()) {
        current += path;
        do_mkdir(current);
    }
}

void do_mkdir(std::string path) {
    struct stat st = {0};

    int rc = stat(path.c_str(), &st);

    if (rc != 0) {
        rc = mkdir(path.c_str(), 0755);

        if (rc != 0 && errno != EEXIST) {
            // TODO must throw exception with the propriate message
        }
    } else if (!S_ISDIR(st.st_mode)) {
        std::string error = path + " is not a directory";
        throw std::runtime_error(error.c_str());
    }
}

std::string prompt(const std::string &out) {
    std::string input;

    std::cout << out;

    std::getline(std::cin, input);

    return input;
}

void set_echo(bool echo) {
    struct termios t;

    tcgetattr(stdin->_fileno, &t);

    if (echo) t.c_lflag |= ECHO;
    else t.c_lflag &= ~ECHO;

    tcsetattr(stdin->_fileno, TCSANOW, &t);
}

void print_exception(const std::exception &ex) {
    std::cerr << "[ERROR] " << ex.what() << '\n';
}

std::string password_str(const struct password_t &pw, bool show_content) {
    std::string out = "id : " + std::to_string(pw.id) + "\ntitle: " + pw.title;

    if (show_content) out += "\ncontent : " + pw.content;

    return out;
}

std::string prompt_password() {
    set_echo(false);
    std::string password = prompt("Enter password : ");
    set_echo(true);
    std::cout << '\n';
    return password;
}
