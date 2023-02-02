#include <cstddef>
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

static cxxopts::ParseResult parse_options(int argc, const char *const *argv);
static int interactive_mode(PasswordManager &passmg);
static void interactive_auth_mode(PasswordManager &passmg);
static void print_auth_menu();
static int try_login(PasswordManager &passmg, std::string username,
                     std::string password);
static int try_signup(PasswordManager &passmg, std::string username,
                      std::string password);
static void print_menu();
static void create_password(PasswordManager &passmg);
static void list_passwords(PasswordManager &passmg);
static void get_password(PasswordManager &passmg);
static void remove_password(PasswordManager &passmg);
static void print_banner();

int main(int argc, const char *const *argv) {
    cxxopts::ParseResult results = parse_options(argc, argv);
    fs::path db_dir = fs::path(passmg_getenv("HOME")) / PASSMG_HOME / "data/";
    fs::path db_path = db_dir / "db.sqlite";
    std::string username, password;
    int rc = 0;

    if (results.count("version")) {
        std::cout << get_version() << '\n';
        return EXIT_SUCCESS;
    }

    mkpath(db_dir);

    PasswordManager passmg(db_path);

    if (results.count("interactive")) return interactive_mode(passmg);

    username = results.count("username") ? results["username"].as<std::string>()
                                         : prompt("Enter username : ");

    if (results.count("password")) {
        password = results["password"].as<std::string>();
    } else {
        password = prompt_password();
    }

    if (results.count("signup")) {
        rc = try_signup(passmg, username, password);
    } else {
        rc = try_login(passmg, username, password);
    }

    if (rc != EXIT_SUCCESS) exit(rc);

    if (results.count("list")) {
        list_passwords(passmg);
        return EXIT_SUCCESS;
    }

    if (results.count("create")) {
        std::string title = results.count("title") > 0
                                ? results["title"].as<std::string>()
                                : "";
        std::string content = results.count("content") > 0
                                  ? results["content"].as<std::string>()
                                  : "";

        if (title.length() <= 0) {
            std::cerr << "[ERROR] you must provide a title using --title\n";
            return EXIT_FAILURE;
        }

        if (content.length() <= 0) {
            std::cerr << "[ERROR] you must provide the password content using "
                         "--content\n";
            return EXIT_FAILURE;
        }

        try {
            struct password_t pw = passmg.create(title, content);
            std::cout << "[INFO] password has been created\n"
                      << password_str(pw, true) << '\n';
        } catch (std::exception &ex) {
            print_exception(ex);
        }

        return EXIT_SUCCESS;
    }

    if (results.count("get") > 0) {
        std::string title = results.count("title") > 0
                                ? results["title"].as<std::string>()
                                : "";
        std::size_t id =
            results.count("id") > 0 ? results["id"].as<std::size_t>() : 0;
        struct password_t pw = {0};

        if (title.length() <= 0 && id == 0) {
            std::cerr << "[ERROR] must provide password title or id\n";
            return EXIT_FAILURE;
        }

        if (title.length() > 0) pw = passmg.get(title);
        else if (id > 0) pw = passmg.get(id);

        if (pw.id <= 0) {
            std::cerr << "[ERROR] password not found\n";
            return EXIT_FAILURE;
        }

        std::cout << password_str(pw, true) << '\n';
        return EXIT_SUCCESS;
    }

    if (results.count("remove") > 0) {
        std::string title = results.count("title") > 0
                                ? results["title"].as<std::string>()
                                : "";
        std::size_t id =
            results.count("id") > 0 ? results["id"].as<std::size_t>() : 0;

        if (title.length() <= 0 && id == 0) {
            std::cerr << "[ERROR] must provide password title or id\n";
            return EXIT_FAILURE;
        }

        if (title.length() > 0) passmg.remove(title);
        else if (id > 0) passmg.remove(id);

        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

cxxopts::ParseResult parse_options(int argc, const char *const *argv) {
    cxxopts::ParseResult results;
    cxxopts::Options options("Password Manager",
                             "A simple local password manager");

    // clang-format off
    options.add_options()
        ("i,interactive", "Enter interactive mode")
        ("signup", "Create an account")
        ("l,list", "List saved password")
        ("c,create", "Create new password")
        ("get", "Get content of password")
        ("d,remove", "Delete password")
        ("title", "Password title", cxxopts::value<std::string>())
        ("id", "Password id", cxxopts::value<std::size_t>())
        ("content", "Password content", cxxopts::value<std::string>())
        ("u,username","Authentication username",cxxopts::value<std::string>())
        ("p,password","Authentication password",cxxopts::value<std::string>())
        ("v,version", "show version")
        ("h,help", "show help");
    // clang-format on

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
    print_banner();
    interactive_auth_mode(passmg);

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
        } else if (cmd.compare("get") == 0) {
            get_password(passmg);
        } else if (cmd.compare("rm") == 0 || cmd.compare("remove") == 0) {
            remove_password(passmg);
        }
    }

    return EXIT_SUCCESS;
}

void interactive_auth_mode(PasswordManager &passmg) {
    while (!passmg.is_authenticated()) {
        std::string input = prompt("> ");

        if (input.compare("help") == 0 || input.compare("h") == 0) {
            print_auth_menu();
        } else if (input.compare("quit") == 0 || input.compare("q") == 0) {
            exit(EXIT_SUCCESS);
        } else if (input.compare("login") == 0) {
            std::string username = prompt("Enter username : ");
            std::string password = prompt_password();
            try_login(passmg, username, password);
        } else if (input.compare("signup") == 0) {
            std::string username = prompt("Enter username : ");
            std::string password = prompt_password();
            try_signup(passmg, username, password);
        }
    }
}

void print_auth_menu() {
    std::cout << "signup - Create account" << '\n'
              << "login" << '\n'
              << "help" << '\n'
              << "quit" << '\n';
}

void print_menu() {
    std::cout << "help - show help\n"
              << "create - create a new password\n"
              << "get - get content of a password\n"
              << "remove - delete a password\n"
              << "list - list saved passwords\n"
              << "quit\n";
}

int try_login(PasswordManager &passmg, std::string username,
              std::string password) {
    try {
        passmg.login(username, password);
        return EXIT_SUCCESS;
    } catch (PasswordManagerException &ex) {
        print_exception(ex);
    } catch (std::exception &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    }

    return EXIT_FAILURE;
}

int try_signup(PasswordManager &passmg, std::string username,
               std::string password) {
    try {
        passmg.signup(username, password);
        return EXIT_SUCCESS;
    } catch (PasswordManagerException &ex) {
        print_exception(ex);
    } catch (std::exception &ex) {
        print_exception(ex);
        exit(EXIT_FAILURE);
    }

    return EXIT_FAILURE;
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
    std::vector<struct password_t> pws = passmg.list(true);

    for (auto pw : pws) {
        std::cout << password_str(pw, false) << '\n' << repeat("+", 20) << '\n';
    }
}

void get_password(PasswordManager &passmg) {
    std::string title = prompt("Enter password title : ");
    struct password_t password = {0};

    if (title.length() <= 0) return;

    password = passmg.get(title);

    if (password.id <= 0) {
        std::cerr << "[ERROR] Password doesn't exist\n";
        return;
    }

    std::cout << password_str(password, true) << '\n';
}

void remove_password(PasswordManager &passmg) {
    std::string title = prompt("Enter password title : ");
    struct password_t password = {0};

    if (title.length() <= 0) return;

    passmg.remove(title);

    // TODO MUST check if password has been deleted and pring message
}

static void print_banner() {
    std::cout << get_version() << '\n' << "type 'help' to display help menu\n";
}
