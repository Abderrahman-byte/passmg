#include <exception>
#include <string>

#ifndef _UTILS_PASSMG_CLI_HPP_
#define _UTILS_PASSMG_CLI_HPP_

extern std::string get_version();
extern std::string passmg_getenv(const char *env);
extern void do_mkdir(std::string path);
extern void mkpath(std::string path);
extern std::string prompt(const std::string &out);
extern void set_echo(bool echo);
extern void print_exception(const std::exception &ex);

#endif
