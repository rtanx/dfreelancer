#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ui.h"
#include "app.h"

CONSOLE_SCREEN_BUFFER_INFO csbi;

struct Winsize
{
    int rows, cols;
} wsize;

void refill_wsize()
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    wsize.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    wsize.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

int print_banner()
{
    FILE *fp = fopen(BANNERPATH, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "error opening %s\n", BANNERPATH);
        return 1;
    }
    size_t strsize = 1e7 * sizeof(char);
    char *rstr = (char *)malloc(strsize);
    int i = 0;
    while (fgets(rstr, strsize, fp) != NULL)
    {
        printf("%s\t\t%s", (i % 2 == 0 ? C_BGREEN : C_BRED), rstr);
        i++;
    }
    printf(C_RESET);
    fclose(fp);
    free(rstr);
    return 0;
}

void printborder(size_t len)
{
    char *s = (char *)malloc((len + 1) * sizeof(char));
    memset(s, '-', len);
    s[len] = '\0';
    printf("+%s+\n", s);
    free(s);
}
void print_subopt()
{
    printf("\n98. Back | 99. Exit\n");
}
void print_invld_opt()
{
    printf("Option tidak valid\n");
}
void printcenter(char *text, int fieldWidth)
{

    int padlen = CALCPAD(fieldWidth, strlen(text)); //(fieldWidth - strlen(text)) / 2;
    printf("%*s%s%*s", padlen, "", text, padlen, "");
}

void index_view()
{
    system("cls");
    print_banner();
    refill_wsize();
    printcenter(C_BGREEN "======= WELCOME TO D'FREELANCER =======" C_RESET, wsize.cols);
    printf("\n\n\n");
    system("pause");
    system("cls");
    sign_method();
}

void sign_method()
{
    system("cls");
    char htxt[19] = "PILIH METODE MASUK";
    char opt1[10] = " 1. Login";
    char opt2[13] = " 2. Register";
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 19);
    printf(C_BGREEN);
    printborder(blen);
    printf("|%*s" C_RESET "%s" C_BGREEN "%*s |\n", hpad + 1, " ", htxt, hpad, " ");
    printborder(blen);
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt1, blen - 10, " ");
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt2, blen - 13, " ");
    printborder(blen);
    printf(C_RESET);
    print_subopt();
    int opt;
opt_input:
    printf(">>> ");
    scanf("%d", &opt);
    getchar();
    switch (opt)
    {
    case 1:
        login_opt_view();
        break;
    case 2:
        register_view();
        break;
    case OPT_BACK:
        index_view();
        break;
    case OPT_EXIT:
        exit_program();
        break;
    default:
        print_invld_opt();
        goto opt_input;
    }
}

void login_opt_view()
{
    system("clear");
    char htxt[6] = "LOGIN",
         opt1[23] = " 1. Login Sebagai User",
         opt2[24] = " 2. Login Sebagai Admin";
    refill_wsize();
    int blen = wsize.cols / 2;
    ROLE opt;
    int hpad = CALCPAD(blen, 6);
    printf(C_BGREEN);
    printborder(blen);
    printf("|%*s" C_RESET "%s" C_BGREEN "%*s |\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt1, blen - 23, " ");
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt2, blen - 24, " ");
    printborder(blen);
    printf(C_RESET);
    print_subopt();
opt_input:
    printf(">>> ");
    scanf("%d", &opt);
    getchar();

    switch (opt)
    {
    case USER:
        login_view(USER);
        break;
    case ADMIN:
        login_view(ADMIN);
        break;
    case OPT_BACK:
        sign_method();
        break;
    case OPT_EXIT:
        exit_program();
        break;
    default:
        print_invld_opt();
        goto opt_input;
    }
}
void login_view(ROLE r)
{
    system("cls");
    char uname[MAX_UNAME], pw[MAX_PW];
    char headerText[12] = "USER LOGIN";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 11);
    printf(C_BRED);
    printborder(blen);
    printf("|" C_RESET "%*s%s%*s" C_BRED "|\n", hpad + 1, " ", headerText, hpad + 1, " ");
    printborder(blen);
    printf(C_RESET);
    printf("Masukan username :\n>>> ");
    fgets(uname, MAX_UNAME, stdin);
    printf("Masukan password :\n>>> ");
    getpwd(pw, MAX_PW);
}

void register_view()
{
    system("clear");
    char htxt[13] = "USER RGISTER";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 13);
    printf(C_BRED);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s " C_BRED "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);

    struct User user;
    printf(C_BYELLOW "Username:" C_RESET "\n>>> ");
    getstring(user.username, MAX_UNAME);
    printf(C_BYELLOW "Password:" C_RESET "\n>>> ");
    getpwd(user.password, MAX_PW);
    printf(C_BYELLOW "\nNama:" C_RESET "\n>>> ");
    getstring(user.name, MAX_NAME);
    printf(C_BYELLOW "Alamat:" C_RESET "\n>>> ");
    getstring(user.address, MAX_ADDRESS);
    printf(C_BYELLOW "Nomor Telepon:" C_RESET "\n>>> ");
    getstring(user.phone, MAX_PHONE);
    user_register(user);
    system("pause");
}

void exit_program()
{
    printf("bye...\n");
}