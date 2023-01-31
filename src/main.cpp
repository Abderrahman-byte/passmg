#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <vector>

#include "cxxopts.hpp"

#include "libpassmg/PasswordManager.hpp"
#include "libpassmg/config.hpp"
#include "libpassmg/exceptions.hpp"
#include "libpassmg/utils.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

cxxopts::ParseResult parse_options(int argc, const char *const *argv);
int interactive_mode(PasswordManager &passmg);
void auth_menu(PasswordManager &passmg);
void print_auth_menu();
void try_login(PasswordManager &passmg, std::string username,
               std::string password);
void try_signup(PasswordManager &passmg, std::string username,
                std::string password);
void print_menu();
void create_password(PasswordManager &passmg);
void list_passwords(PasswordManager &passmg);

int main(int argc, const char *const *argv) {
    cxxopts::ParseResult results = parse_options(argc, argv);
    fs::path db_dir = fs::path(passmg_getenv("HOME")) / PASSMG_HOME / "data/";
    fs::path db_path = db_dir / "db.sqlite";
    std::string username, password;

    if (results.count("version")) {
        std::cout << get_version() << '\n';
        return EXIT_SUCCESS;
    }

    mkpath(db_dir);

    PasswordManager passmg(db_path);

    if (results.count("interactive")) return interactive_mode(passmg);

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

    while (true) {
        std::string cmd = prompt(passmg.get_username() + " > ");

        if (cmd.compare("q") == 0 || cmd.compare("quit") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (cmd.compare("h") == 0 || cmd.compare("help") == 0) {
            print_menu();
        } else if (cmd.compare("create") == 0) {
            create_password(passmg);
        } else if (cmd.compare("list") == 0) {
            list_passwords(passmg);
        }
    }

    return EXIT_SUCCESS;
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

void print_menu() {
    std::cout << "help - show help\n"
              << "quit\n"
              << "create - create a new password\n"
              << "get - get content of a password\n"
              << "list - list saved passwords\n";
}

void try_login(PasswordManager &passmg, std::string username,
               std::string password) {
    try {
        passmg.login(username, password);
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
    } catch (PasswordManagerException &ex) {
        print_exception(ex);
    } catch (std::exception &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    }
}

void create_password(PasswordManager &passmg) {
    std::string title, content, resp;

    title = prompt("Enter new password title : ");

    resp = prompt("Do want to auto generate the password (Y/n) ? ");

    if (resp.compare("y") == 0 || resp.compare("Y") == 0) {
        content = random_str(RANDOM_PASSWORD_LENGTH);
    } else {
        content = prompt("Enter password : ");
    }

    if (content.empty()) return;

    try {
        struct password_t pw = passmg.create(title, content);
        std::cout << "[INFO] password has been created\n"
                  << password_str(pw, true) << '\n';
    } catch (std::exception &ex) {
        print_exception(ex);
    }
}

void list_passwords(PasswordManager &passmg) {
    std::vector<struct password_t> pws = passmg.list();

    for (auto pw : pws) {
        std::cout << password_str(pw, false) << "\n++++++++++++++++++" << '\n';
    }
}
