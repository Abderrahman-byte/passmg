#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>

#include "cxxopts.hpp"

#include "libpassmg/PasswordManager.hpp"
#include "libpassmg/config.hpp"
#include "libpassmg/exceptions.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

fs::path get_home_dir();
cxxopts::ParseResult parse_options(int argc, const char *const *argv);
std::string prompt_password();
int interactive_mode(PasswordManager &passmg);
void auth_menu(PasswordManager &passmg);
void print_auth_menu();
void try_login(PasswordManager &passmg, std::string username,
               std::string password);
void try_signup(PasswordManager &passmg, std::string username,
                std::string password);
void print_menu();

int main(int argc, const char *const *argv) {
    cxxopts::ParseResult results = parse_options(argc, argv);
    fs::path home_dir = get_home_dir();
    fs::path db_dir = home_dir / "data/";
    fs::path db_path = db_dir / "db.sqlite";
    std::string username, password;

    if (results.count("version")) {
        std::cout << get_version() << '\n';
        return EXIT_SUCCESS;
    }

    mkpath(db_dir);

    PasswordManager passmg(db_path);

    interactive_mode(passmg);

    /* username = results.count("username") ?
     * results["username"].as<std::string>() */
    /*                                      : prompt("Enter username : "); */

    /* if (results.count("password")) { */
    /*     password = results["password"].as<std::string>(); */
    /* } else { */
    /*     password = prompt_password(); */
    /* } */

    /* std::cout << "username : " << username << std::endl; */
    /* std::cout << "password : " << password << std::endl; */

    return EXIT_SUCCESS;
}

fs::path get_home_dir() {
    auto home_dir = fs::path(passmg_getenv("HOME")) / PASSMG_HOME;

    return home_dir;
}

std::string prompt_password() {
    set_echo(false);
    std::string password = prompt("Enter password : ");
    set_echo(true);
    std::cout << '\n';
    return password;
}

cxxopts::ParseResult parse_options(int argc, const char *const *argv) {
    cxxopts::ParseResult results;
    cxxopts::Options options("Password Manager",
                             "A simple local password manager");

    // clang-format off
    options.add_options()
        ("command", "Action commands : list, get, delete, add", cxxopts::value<std::string>())
        ("args", "arguments of the command", cxxopts::value<std::vector<std::string>>())
        ("i,interactive", "Enter interactive mode")
        ("u,username","Authentication username",cxxopts::value<std::string>())
        ("p,password","Authentication password",cxxopts::value<std::string>())
        ("v,version", "show version")
        ("h,help", "show help");
    // clang-format on

    options.parse_positional({"command", "args"});
    options.positional_help("[command] [argument]");

    options.show_positional_help();

    try {
        results = options.parse(argc, argv);
    } catch (const cxxopts::OptionParseException &ex) {
        std::cerr << ex.what() << '\n';
        std::cerr << options.help();
        exit(EXIT_FAILURE);
    }

    if (results.count("help")) {
        std::cout << options.help();
        exit(EXIT_SUCCESS);
    }

    return results;
}

int interactive_mode(PasswordManager &passmg) {
    auth_menu(passmg);

    if (!passmg.is_authenticated()) return EXIT_FAILURE;

    return 0;
}

void auth_menu(PasswordManager &passmg) {
    while (!passmg.is_authenticated()) {
        std::string input = prompt("> ");

        if (input.compare("help") == 0 || input.compare("h") == 0) {
            print_auth_menu();
        } else if (input.compare("quit") == 0 || input.compare("q") == 0) {
            exit(EXIT_SUCCESS);
        } else if (input.compare("1") == 0 || input.compare("login") == 0) {
            std::string username = prompt("Enter username : ");
            std::string password = prompt_password();
            try_login(passmg, username, password);
        } else if (input.compare("2") == 0 || input.compare("signup") == 0) {
            std::string username = prompt("Enter username : ");
            std::string password = prompt_password();
            try_signup(passmg, username, password);
        }
    }
}

void print_auth_menu() {
    std::cout << "1 - Login" << '\n'
              << "2 - Create account" << '\n'
              << "h - help" << '\n'
              << "q - quit" << '\n';
}

void print_menu() {}

void try_login(PasswordManager &passmg, std::string username,
               std::string password) {
    try {
        passmg.login(username, password);
    } catch (DatabaseException &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    } catch (PasswordManagerException &ex) {
        print_exception(ex);
    } catch (std::exception &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    }
}

void try_signup(PasswordManager &passmg, std::string username,
                std::string password) {
    try {
        passmg.signup(username, password);
    } catch (DatabaseException &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    } catch (PasswordManagerException &ex) {
        print_exception(ex);
    } catch (std::exception &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    }
}
