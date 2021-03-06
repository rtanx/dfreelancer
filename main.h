#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include "app.h"

#define C_RESET "\033[0m"
#define C_BGREEN "\033[1;32m"
#define C_BRED "\033[1;31m"
#define C_BCYAN "\033[1;36m"
#define C_BYELLOW "\033[1;33m"
#define BANNERPATH "banner.txt"
#define CALCPAD(w, slen) ((w - slen) / 2)
#define OPT_LOGOUT 97
#define OPT_BACK 98
#define OPT_EXIT 99

void login_opt_view();
void login_view(ROLE r);
void index_view();
void register_view();
void sign_method();
void menu_services_view();
void buy_menu_view();
void sell_menu_view();
void register_service_view();
void global_announcement_view();
void admin_menu_view();
void admin_services_list_view();
void admin_user_data_list_view();

void printcenter(char *text, int fieldWidth);
int print_banner();
void printborder(size_t len);
void print_invld_opt();
void print_subopt();
void exit_program();
void logout();
#endif