#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define CHREP (buf, size, ch) memset(&buf, ch, size)
#define RESET_TEXT_COLOR "\033[0m"
#define C_BGREEN "\033[1;32m"
#define C_BRED "\033[1;31m"
#define BANNERPATH "banner.txt"
#define CALCPAD(w, slen) ((w - slen) / 2)
#define BACK_OPT 98
#define EXIT_OPT 99
int print_banner();
void justifyprint(char *s);
void printcenter(char *text, int fieldWidth);
void login_handler();
void register_handler();
void printborder();
void landing_page();
void printtable();
void exit_program();
void print_subopt();
struct winsize wsize; // store the current windows size
int main()
{
    system("clear");
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);
    print_banner();
    // char *wlcmtxt = (char *)malloc(10 * sizeof(char));
    // free(wlcmtxt);

    printcenter(C_BGREEN "======= WELCOME TO D'FREELANCER =======" RESET_TEXT_COLOR, wsize.ws_col);
    printf("\n\n\n");
    printf("Press Any Key to continue...");
    getchar();
    system("clear");
    landing_page();
    // printtable();
    return 0;
}
void printcenter(char *text, int fieldWidth)
{
    // char nl = ;
    // int fnl = 0;
    // if (newline)

    int padlen = CALCPAD(fieldWidth, strlen(text)); //(fieldWidth - strlen(text)) / 2;
    printf("%*s%s%*s", padlen, "", text, padlen, "");
}
void exit_program()
{
    printf("Program exited..\n");
    exit(0);
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
    printf(RESET_TEXT_COLOR);
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
void landing_page()
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);
    int blen = 24, opt;
    int hpad = CALCPAD(blen, 16);
    printborder(blen);
    printf("|%*sPILIH OPSI MASUK%*s|\n", hpad, " ", hpad, " ");
    printborder(blen);
    printf("| 1.  Register\t\t |\n");
    printf("| 2.  Login\t\t |\n");
    printborder(blen);
    print_subopt();
    printf("> ");
    scanf("%u", &opt);
    getchar();
    switch (opt)
    {
    case 1:
        register_handler();
        break;
    case 2:
        login_handler();
        break;
    case EXIT_OPT:
        exit_program();
        break;
    case BACK_OPT:
        main();
        break;
    default:
        printf("salahh\n");
        break;
    }
}
void login_handler()
{
    system("clear");
    int blen = 24, opt;
    int hpad = CALCPAD(blen, 5);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" RESET_TEXT_COLOR "%*sLOGIN%*s" C_BGREEN "|\n", hpad + 1, " ", hpad, " ");
    printborder(blen);
    printf("|" RESET_TEXT_COLOR " 1. Login Sebagai User  " C_BGREEN "|\n");
    printf("|" RESET_TEXT_COLOR " 2. Login Sebagai Admin " C_BGREEN "|\n");
    printborder(blen);
    printf(RESET_TEXT_COLOR);
    print_subopt();
    printf("> ");
    scanf("%d", &opt);
    getchar();
}
void register_handler()
{
}