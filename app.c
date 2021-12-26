#include "app.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <errno.h>
#include <shlwapi.h>

extern int errno;

const struct
{
    char *uname;
    char *pw;
} admin_creds = {"admin", "admin"};

REGLOG_ERR user_login(char *uname, char *pw)
{
    FILE *f;
    struct User user;
    f = fopen(USER_DB_PATH, "rb");
    REGLOG_ERR res = CREDEENTIALS_NOT_MATCH;
    while (fread(&user, sizeof(struct User), 1, f))
    {
        if ((strcmp(user.username, uname) == 0) && (strcmp(user.password, pw) == 0))
        {
            // strcpy(LOGGED_USER.user.username, user.username);
            LOGGED_USER.user = user;
            LOGGED_USER.role = USER;
            LOGGED_USER.logged = 1;
            res = NO_ERR;
            break;
        }
    }
    fclose(f);
    return res;
}
bool query_user(struct User u, char *q)
{
    return (StrStrIA(u.username, q) != NULL) ||
           (StrStrIA(u.address, q) != NULL) ||
           (StrStrIA(u.name, q) != NULL) ||
           (StrStrIA(u.phone, q) != NULL);
}
bool query_service(struct Service s, char *q)
{
    return (StrStrIA(s.username, q) != NULL) ||
           (StrStrIA(s.title, q) != NULL) ||
           (StrStrIA(s.info, q) != NULL) ||
           (StrStrIA(s.contact, q) != NULL) ||
           (StrStrIA(s.category, q) != NULL);
}
void read_users(char *filterSearch)
{
    struct User user;
    FILE *fp;
    fp = fopen(USER_DB_PATH, "rb");
    size_t i = 0;
    while (fread(&user, sizeof(struct User), 1, fp) == 1)
    {
        if (filterSearch[0] != '\0')
        {
            if (!query_user(user, filterSearch))
            {
                continue;
            }
        }

        printf("%d. %-20s: %s\n", i + 1, "Username", user.username);
        printf("   %-20s: %s\n", "Nama", user.name);
        printf("   %-20s: %s\n", "Alamat", user.address);
        printf("   %-20s: %s\n", "Kontak", user.phone);
        printf("\n");
        i++;
    }
    fclose(fp);
}
void clear_session()
{
    memset(&LOGGED_USER.user, 0, sizeof(struct User));
    LOGGED_USER.logged = 0;
}
REGLOG_ERR admin_login(char *uname, char *pw)
{
    REGLOG_ERR res = CREDEENTIALS_NOT_MATCH;
    if ((strcmp(uname, admin_creds.uname) == 0) && (strcmp(pw, admin_creds.pw) == 0))
    {
        res = NO_ERR;
        LOGGED_USER.logged = 1;
        memset(&LOGGED_USER.user, 0, sizeof(struct User));
        LOGGED_USER.role = ADMIN;
    }
    return res;
}
int regist_service(struct Service *srvc)
{
    FILE *f;
    f = fopen(SERVICE_DB_PATH, "ab");
    fwrite(srvc, sizeof(struct Service), 1, f);
    fclose(f);
    return 0;
}
// struct Transaction *read_announcement(size_t *sz)
void read_announcement()
{
    FILE *f;
    f = fopen(TRANSACTION_DB_PATH, "rb");
    struct Transaction tr;
    while (fread(&tr, sizeof(struct Transaction), 1, f) == 1)
    {
        if (strcmp(tr.service.username, LOGGED_USER.user.username) == 0)
        {
            printf("%-20s: %s\n", "Username", tr.buyer.username);
            printf("%-20s: %s\n", "Nama", tr.buyer.name);
            printf("%-20s: %s\n", "Alamat", tr.buyer.address);
            printf("%-20s: %s\n", "Kontak", tr.buyer.phone);
            printf("%-20s: %s\n", "Jasa yang dibeli", tr.service.title);
            printf("\n");
        }
    }
    fclose(f);
}
struct Service *read_services(size_t *sz)
{
    FILE *f;
    f = fopen(SERVICE_DB_PATH, "rb");
    struct Service srvc;
    if (f == NULL)
    {
        perror("Kesalahan saat membaca database");
    }
    struct Service *srvclist = malloc(MAX_SERVICE_LIST * sizeof(struct Service));
    size_t n = 0;
    while (fread(&srvc, sizeof(struct Service), 1, f) == 1)
    {
        srvclist[n] = srvc;
        n++;
    }
    sort_services(srvclist, n);

    *sz = n;
    fclose(f);
    return srvclist;
}
int delete_users(char *username)
{
    int del_count = 0;
    FILE *fp, *fp_bak;
    fp = fopen(USER_DB_PATH, "rb");
    fp_bak = fopen(USER_DB_BAK_PATH, "wb");
    struct User user;
    while (fread(&user, sizeof(struct User), 1, fp) == 1)
    {
        if (strcmp(user.username, username) == 0)
        {
            del_count++;
            continue;
        }
        fwrite(&user, sizeof(struct User), 1, fp_bak);
    }
    fclose(fp);
    fclose(fp_bak);

    remove(USER_DB_PATH);
    rename(USER_DB_BAK_PATH, USER_DB_PATH);
    return del_count;
}
void delete_service(struct Service s)
{
    FILE *fp, *fp_bak;
    fp = fopen(SERVICE_DB_PATH, "rb");
    fp_bak = fopen(SERVICE_DB_BAK_PATH, "ab");
    struct Service s2;
    while (fread(&s2, sizeof(struct Service), 1, fp) == 1)
    {
        if (!compare_services(s, s2))
        {
            fwrite(&s2, sizeof(struct Service), 1, fp_bak);
        }
    }
    fclose(fp);
    fclose(fp_bak);

    remove(SERVICE_DB_PATH);
    rename(SERVICE_DB_BAK_PATH, SERVICE_DB_PATH);
}
bool compare_services(struct Service s1, struct Service s2)
{
    return (strcmp(s1.username, s2.username) == 0) &&
           (strcmp(s1.title, s2.title) == 0) &&
           (strcmp(s1.info, s2.info) == 0) &&
           (strcmp(s1.contact, s2.contact) == 0) &&
           (strcmp(s1.category, s2.category) == 0) &&
           s1.price == s2.price;
}

void close_read_services(struct Service *srvclist)
{
    free(srvclist);
}
void sort_services(struct Service *s, size_t size)
{
    struct Service temp;

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size - (i + 1); j++)
        {
            if (strcmp(s[j].title, s[j + 1].title) > 0)
            {
                temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }
}
int is_user_exist(char *uname)
{
    FILE *f;
    struct User user;
    f = fopen(USER_DB_PATH, "rb");
    while (fread(&user, sizeof(struct User), 1, f))
    {
        if (strcmp(user.username, uname) == 0)
            return 1;
    }
    fclose(f);
    return 0;
}
REGLOG_ERR user_register(struct User *userInfo)
{
    if (is_user_exist(userInfo->username))
    {
        return USERNAME_ALREADY_EXIST_ERR;
    }

    FILE *f;
    f = fopen(USER_DB_PATH, "ab");
    fwrite(userInfo, sizeof(struct User), 1, f);
    fclose(f);
    return NO_ERR;
}
void buy(struct Service srvc)
{
    FILE *f;
    struct Transaction tr;
    tr.service = srvc;
    tr.buyer = LOGGED_USER.user;
    f = fopen(TRANSACTION_DB_PATH, "ab");
    fwrite(&tr, sizeof(struct Transaction), 1, f);
    fclose(f);
}
int getstring(char *s, size_t size)
{
    if (fgets(s, size, stdin) == NULL)
        printf("Error reading string.\n");

    int matchend = strcspn(s, "\r\n");
    s[matchend] = 0;
    return matchend + 1;
}
void getpwd(char *s, size_t len)
{
    int c, i = 0;
    while (((c = getch()) != '\r') && i < (len - 1))
    {
        s[i] = (char)c;
        printf("*");
        i++;
    }
    s[i] = '\0';
}