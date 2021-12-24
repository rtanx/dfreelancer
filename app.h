#ifndef APP_H
#define APP_H

typedef unsigned short int ROLE;
#include <stdlib.h>

#define USER (ROLE)1
#define ADMIN (ROLE)2
#define MAX_UNAME 30
#define MIN_UNAME 4
#define MAX_PW 13
#define MIN_PW 7
#define MAX_NAME 60
#define MAX_ADDRESS 256
#define MAX_PHONE 16

struct User
{
    char username[MAX_UNAME],
        password[MAX_PW],
        name[MAX_NAME],
        address[MAX_ADDRESS],
        phone[MAX_PHONE];
};

void login_handler(ROLE);
void user_login();
void user_register(struct User userInfo);
void admin_login();
void getpwd(char *s, size_t len);
int getstring(char *s, size_t size);

#endif