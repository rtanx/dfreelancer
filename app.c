#include "app.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
const struct
{
    char *uname;
    char *pw;
} admin_creds = {"Admin", "rahasiaadmin"};

void user_login()
{
    printf("valid user\n");
}
void admin_login()
{
    printf("valid admin\n");
}
void user_register(struct User userInfo)
{
    userInfo
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