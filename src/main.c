#include "header.h"

int main()
{
    sqlite3 *db = NULL;
    initDatabase("./data/data.db", &db);

    User user;

    if (initMenu(&user, db))
    {
        system("clear");
        printf("error on registation system\n");
        exit(1);
    }

    if (mainMenu(user, db))
    {
        system("clear");
        printf("error on interface system\n");
        exit(1);
    }

    return 0;
}