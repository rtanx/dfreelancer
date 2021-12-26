#ifndef APP_H
#define APP_H

typedef unsigned short int ROLE;
#include <stdlib.h>
#include <stdbool.h>

#define USER (ROLE)1
#define ADMIN (ROLE)2
#define MAX_UNAME 30
#define MIN_UNAME 4
#define MAX_PW 13
#define MIN_PW 7
#define MAX_NAME 60
#define MAX_ADDRESS 256
#define MAX_PHONE 16
#define MAX_TITLE 100
#define MAX_CATEGORY 30
#define MAX_INFO 1024
#define MAX_SERVICE_LIST 1024
#define USER_DB_PATH "user_db.dat"
#define USER_DB_BAK_PATH "user_db_bak.dat"
#define SERVICE_DB_PATH "service_db.dat"
#define SERVICE_DB_BAK_PATH "service_db_bak.dat"
#define TRANSACTION_DB_PATH "transaction_db.dat"

typedef enum REGLOG_ERR
{
    USERNAME_ALREADY_EXIST_ERR,
    CREDEENTIALS_NOT_MATCH,
    NO_ERR
} REGLOG_ERR;

struct User
{
    char username[MAX_UNAME],
        password[MAX_PW],
        name[MAX_NAME],
        address[MAX_ADDRESS],
        phone[MAX_PHONE];
};
struct Logged_user
{
    struct User user;
    ROLE role;
    int logged;
};
struct Logged_user LOGGED_USER;
static const struct Logged_user emptyUser;

struct Service
{
    char title[MAX_TITLE],
        category[MAX_CATEGORY],
        info[MAX_INFO],
        contact[MAX_PHONE],
        username[MAX_UNAME];
    float price;
};
struct Transaction
{
    struct User buyer;
    struct Service service;
};

void login_handler(ROLE);
enum REGLOG_ERR user_login(char *uname, char *pw);
enum REGLOG_ERR user_register(struct User *userInfo);
REGLOG_ERR admin_login(char *uname, char *pw);
void getpwd(char *s, size_t len);
int getstring(char *s, size_t size);
int regist_service(struct Service *srvc);
struct Service *read_services(size_t *sz);
void sort_services(struct Service *s, size_t size);
void close_read_services(struct Service *srvclist);
void buy(struct Service srvc);
void read_announcement();
void read_users(char *filterSearch);
int delete_users(char *username);
bool query_service(struct Service s, char *q);
bool query_user(struct User u, char *q);
bool compare_services(struct Service s1, struct Service s2);
void delete_service(struct Service s);
void clear_session();
#endif