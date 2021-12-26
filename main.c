#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "app.h"

CONSOLE_SCREEN_BUFFER_INFO csbi;

struct Winsize
{
    int rows, cols;
} wsize;
char filterCategory[MAX_CATEGORY] = "";
char filterSearch[512];

int main()
{
    index_view();
}

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
    if (LOGGED_USER.logged == 1)
    {
        printf("\n97. Logout | 98. Back | 99. Exit\n");
        return;
    }

    printf("\n98. Back | 99. Exit\n");
    return;
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
    if (LOGGED_USER.logged == 1)
    {
        if (LOGGED_USER.role == ADMIN)
        {
            admin_menu_view();
        }
        else
        {
            menu_services_view();
        }

        return;
    }

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
    case OPT_LOGOUT:
        logout();
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
    int opt;
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
    char htxt[13] = "USER LOGIN";
    size_t htxt_len = 11;
    if (r == ADMIN)
    {
        htxt_len = 12;
        strcpy(htxt, "ADMIN LOGIN");
    }

    char uname[MAX_UNAME], pw[MAX_PW];
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, htxt_len);
    printf(C_BRED);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s " C_BRED "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);
    short int try = 1;
input:
    printf("Masukan username :\n>>> ");
    getstring(uname, MAX_UNAME);
    printf("Masukan password :\n>>> ");
    getpwd(pw, MAX_PW);
    if (r == ADMIN)
    {
        REGLOG_ERR e = admin_login(uname, pw);
        if (e == CREDEENTIALS_NOT_MATCH)
        {
            printf(C_BRED "\n\nUsername atau password salah!\n" C_RESET);
            goto input;
        }
        admin_menu_view();
    }
    else
    {
        REGLOG_ERR e = user_login(uname, pw);
        if (e == CREDEENTIALS_NOT_MATCH)
        {
            if (try >= 3)
            {
                printf(C_BRED "\n\nUpaya masuk habis!\n\n" C_RESET);
                system("pause");
                index_view();
                return;
            }

            printf(C_BRED "\n\nUsername atau password salah!\n" C_RESET);
            printf(C_BYELLOW "(upaya masuk: %d)\n\n" C_RESET, try++);
            goto input;
            return;
        }
        else
        {
            printf(C_BGREEN "\n\nLogin Berhasil!\n\n" C_RESET);
            system("pause");
            menu_services_view();
            return;
        }
    }
}
void admin_menu_view()
{
    system("cls");
    char htxt[5] = "MENU";
    char opt1[14] = " 1. Data Jasa";
    char opt2[14] = " 2. Data User";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 5);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s " C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt1, blen - 14, " ");
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt2, blen - 14, " ");
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
        admin_services_list_view();
        break;
    case 2:
        admin_user_data_list_view();
        break;
    case OPT_LOGOUT:
        logout();
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
void admin_user_data_list_view()
{
    system("clear");
    char htxt[15] = "LIST DATA USER";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 15);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s" C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);
    read_users(filterSearch);
    printf(C_BGREEN);
    printborder(blen);
    printf(C_RESET);

    printf("\n1. Cari | 2. Tampilkan semua | 3. Hapus | 97. Logout | 98. Back | 99. Exit\n");
    int in;
    char del_u[MAX_UNAME];
inpt:
    printf(">>> ");
    scanf("%d", &in);
    getchar();
    switch (in)
    {
    case 1:
        printf("Ketikan string query:\n>>> ");
        getstring(filterSearch, sizeof(filterSearch) / sizeof(filterSearch[0]));
        admin_user_data_list_view();
        break;
    case 2:
        strcpy(filterSearch, "");
        admin_user_data_list_view();
        break;
    case 3:
        printf("Masukan username user yang ingin dihapus:\n>>> ");
        getstring(del_u, MAX_UNAME);
        if (delete_users(del_u))
        {
            printf(C_BGREEN "Data berhasil dihapus.\n" C_RESET);
        }
        else
        {
            printf(C_BRED "Username yang dimasukan tidak cocok dengan data manapun.\n" C_RESET);
        }
        system("pause");
        admin_user_data_list_view();
        break;
    case OPT_LOGOUT:
        logout();
        break;
    case OPT_BACK:
        admin_menu_view();
        break;
    case OPT_EXIT:
        exit_program();
        break;
    default:
        print_invld_opt();
        goto inpt;
        break;
    }
}
void admin_services_list_view()
{
    system("clear");
    char htxt[15] = "LIST DATA JASA";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 15);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s" C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);

    struct Service *s;
    size_t sz;

    s = read_services(&sz);

    for (size_t i = 0; i < sz; i++)
    {
        if (filterSearch[0] != '\0')
        {
            if (!query_service(s[i], filterSearch))
            {
                continue;
            }
        }
        printf("%-20s: %d\n", "Nomor", i + 1);
        printf("%-20s: %s\n", "Judul", s[i].title);
        printf("%-20s: %s\n", "Kategori", s[i].category);
        printf("%-20s: %s\n", "Info", s[i].info);
        printf("%-20s: %s\n", "Kontak", s[i].contact);
        printf("%-20s: %s\n", "Username Penjual", s[i].username);
        printf("\n");
    }

    printf(C_BGREEN);
    printborder(blen);
    printf(C_RESET);
    printf("\n1. Cari | 2. Tampilkan semua | 3. Hapus | 97. Logout | 98. Back | 99. Exit\n");

    int in;
    size_t del_no;
inpt:
    printf(">>> ");
    scanf("%d", &in);
    getchar();
    switch (in)
    {
    case 1:
        printf("Ketikan string query:\n>>> ");
        getstring(filterSearch, sizeof(filterSearch) / sizeof(filterSearch[0]));
        close_read_services(s);
        admin_services_list_view();
        break;
    case 2:
        strcpy(filterSearch, "");
        close_read_services(s);
        admin_services_list_view();
        break;
    case 3:
    del:
        printf("Masukan Nomor Produk yang ingin dihapus:\n>>> ");
        scanf("%u", &del_no);
        if (!(del_no > 0 && del_no <= sz))
        {
            printf(C_BRED "Nomor produk yang dimasukan tidak valid!\n" C_RESET);
            goto del;
        }
        delete_service(s[del_no - 1]);
        close_read_services(s);
        printf(C_BGREEN "Data berhasil dihapus.\n" C_RESET);
        system("pause");
        admin_services_list_view();
        break;
    case OPT_LOGOUT:
        logout();
        break;
    case OPT_BACK:
        close_read_services(s);
        admin_menu_view();
        break;
    case OPT_EXIT:
        close_read_services(s);
        exit_program();
        break;
    default:
        print_invld_opt();
        goto inpt;
        break;
    }
}
void register_view()
{
    system("clear");
    char htxt[14] = "USER REGISTER";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 14);
    printf(C_BRED);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s" C_BRED "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);

    struct User user;
    printf("\n");
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
    REGLOG_ERR e = user_register(&user);
    if (e == USERNAME_ALREADY_EXIST_ERR)
    {
        printf(C_BRED "\nUsername \"%s\" telah digunakan, silahkan gunakan username lain.\n" C_RESET, user.username);
        system("pause");
        register_view();
    }
    else
    {
        printf(C_BGREEN "\nRegister Berhasil! silahkan login.\n" C_RESET);
        system("pause");
        login_opt_view();
    }

    system("pause");
}
void menu_services_view()
{
    system("cls");
    char htxt[5] = "MENU";
    char opt1[9] = " 1. Beli";
    char opt2[9] = " 2. Jual";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 5);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s " C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt1, blen - 9, " ");
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt2, blen - 9, " ");
    printborder(blen);
    printf(C_RESET);
    print_subopt();

    int in;

input:
    printf(">>> ");
    scanf("%d", &in);
    getchar();

    switch (in)
    {
    case 1:
        buy_menu_view("");
        break;
    case 2:
        sell_menu_view();
        break;
    case OPT_LOGOUT:
        logout();
        break;
    case OPT_BACK:
        index_view();
        break;
    case OPT_EXIT:
        exit_program();
        break;
    default:
        print_invld_opt();
        goto input;
        break;
    }
}
void buy_menu_view()
{
    system("cls");
    char htxt[5] = "BELI";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 5);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s " C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);
    struct Service *s;
    size_t sz;

    s = read_services(&sz);

    for (size_t i = 0; i < sz; i++)
    {
        if (filterCategory[0] != '\0')
        {
            if (strcasecmp(filterCategory, s[i].category) != 0)
            {
                continue;
            }
        }
        printf("%-20s: %d\n", "Nomor", i + 1);
        printf("%-20s: %s\n", "Judul", s[i].title);
        printf("%-20s: %s\n", "Kategori", s[i].category);
        printf("%-20s: %s\n", "Info", s[i].info);
        printf("%-20s: %s\n", "Kontak", s[i].contact);
        printf("%-20s: %s\n", "Username Penjual", s[i].username);
        printf("\n");
    }

    printf(C_BGREEN);
    printborder(blen);
    printf(C_RESET);
    printf("\n1. Beli | 2. Tampilkan semua | 3. Tampilkan berdasarkan kategori | 97. Logout | 98. Back | 99. Exit\n");
    int in;
    size_t buy_no = 0;

inpt:
    printf(">>> ");
    scanf("%d", &in);
    getchar();
    switch (in)
    {
    case 1:
    buy_in:
        printf("Masukan Nomor Produk yang ingin dibeli:\n>>> ");
        scanf("%u", &buy_no);
        getchar();
        if (!(buy_no > 0 && buy_no <= sz))
        {
            printf(C_BRED "Nomor produk yang dimasukan tidak valid!\n" C_RESET);
            goto buy_in;
        }
        buy(s[buy_no - 1]);
        printf(C_BGREEN "\nProduk berhasil dibeli!\n" C_RESET);
        system("pause");
        close_read_services(s);
        buy_menu_view();
        break;
    case 2:
        strcpy(filterCategory, "");
        close_read_services(s);
        buy_menu_view();
        break;
    case 3:
        printf("Ketikan kategori:\n>>> ");
        getstring(filterCategory, MAX_CATEGORY);
        close_read_services(s);
        buy_menu_view();
        break;
    case OPT_LOGOUT:
        logout();
        break;
    case OPT_BACK:
        close_read_services(s);
        menu_services_view();
        break;
    case OPT_EXIT:
        close_read_services(s);
        exit_program();
        break;
    default:
        print_invld_opt();
        goto inpt;
        break;
    }
}
void sell_menu_view()
{
    system("cls");
    char htxt[5] = "JUAL";
    char opt1[19] = " 1. Daftarkan Jasa";
    char opt2[22] = " 2. Pengumuman Global";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 5);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s " C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt1, blen - 19, " ");
    printf("|" C_RESET "%s" C_BGREEN "%*s |\n", opt2, blen - 22, " ");
    printborder(blen);
    printf(C_RESET);
    print_subopt();
    int in;
input:
    printf(">>> ");
    scanf("%d", &in);
    getchar();
    switch (in)
    {
    case 1:
        register_service_view();
        break;
    case 2:
        global_announcement_view();
        break;
    case OPT_LOGOUT:
        logout();
        break;
    case OPT_BACK:
        menu_services_view();
        break;
    case OPT_EXIT:
        exit_program();
        break;
    default:
        print_invld_opt();
        goto input;
        break;
    }
}
void register_service_view()
{
    system("cls");
    char htxt[12] = "DAFTAR JASA";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 12);
    printf(C_BRED);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s" C_BRED "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);

    struct Service srvc;
    printf("\n");
    printf(C_BYELLOW "Judul:" C_RESET "\n>>> ");
    getstring(srvc.title, MAX_TITLE);
    printf(C_BYELLOW "Kategori:" C_RESET "\n>>> ");
    getstring(srvc.category, MAX_CATEGORY);
    printf(C_BYELLOW "Info Jasa:" C_RESET "\n>>> ");
    getstring(srvc.info, MAX_INFO);
    printf(C_BYELLOW "Kontak:" C_RESET "\n>>> ");
    getstring(srvc.contact, MAX_PHONE);
    printf(C_BYELLOW "Harga:" C_RESET "\n>>> ");
    scanf("%f", &srvc.price);
    getchar();
    strcpy(srvc.username, LOGGED_USER.user.username);

    regist_service(&srvc);

    printf(C_BGREEN "\nBerhasil Mendaftarkan Jasa!\n" C_RESET);
    printborder(wsize.cols - 2);
    printf(C_BYELLOW "Judul: " C_RESET "%s\n", srvc.title);
    printf(C_BYELLOW "Kategori: " C_RESET "%s\n", srvc.category);
    printf(C_BYELLOW "Info Jasa: " C_RESET "%s\n", srvc.info);
    printf(C_BYELLOW "Kontak: " C_RESET "%s\n", srvc.contact);
    printf(C_BYELLOW "Harga: " C_RESET "%g\n", srvc.price);
    printborder(wsize.cols - 2);

    system("pause");
    sell_menu_view();
}
void global_announcement_view()
{
    system("cls");
    char htxt[18] = "PENGUMUMAN GLOBAL";
    refill_wsize();
    int blen = wsize.cols / 2;
    int hpad = CALCPAD(blen, 18);
    printf(C_BGREEN);
    printborder(blen);
    printf("|" C_RESET " %*s%s%*s" C_BGREEN "|\n", hpad, " ", htxt, hpad, " ");
    printborder(blen);
    printf(C_RESET);
    read_announcement();

    print_subopt();
    int in;
input:
    printf(">>> ");
    scanf("%d", &in);
    getchar();
    switch (in)
    {
    case OPT_LOGOUT:
        logout();
        break;
    case OPT_BACK:
        sell_menu_view();
        break;
    case OPT_EXIT:
        exit_program();
        break;
    default:
        print_invld_opt();
        goto input;
        break;
    }
}
void logout()
{
    clear_session();
    printf(C_BYELLOW "\nAnda berhasil logout.\n" C_RESET);
    system("pause");
    index_view();
}
void exit_program()
{
    printf("bye...\n");
}