#include <string>

#ifndef _PASS_UTILS_H_
#define _PASS_UTILS_H_
extern bool file_exists(const std::string&);
extern void display_auth_menu();
extern void display_actions_menu();
extern void disable_echo();
extern void echo(bool);
extern std::string convertToString(char *, int);
#endif
