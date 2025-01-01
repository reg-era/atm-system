#include "header.h"

int initMenu(User *user, sqlite3 *db)
{
    system("clear");
    while (1)
    {
        printf(
            "\n\n\t\t======= ATM ======="
            "\n\n\t\t-->> Login/Register:"
            "\n\n\t\t[1]- Login"
            "\n\n\t\t[2]- Register"
            "\n\n\t\t[3]- Exit\n");

        char input[10];
        if (fgets(input, sizeof(input), stdin))
        {
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "1") == 0)
            {
                if (loginMenu(user, db) != 0)
                {
                    system("clear");
                    printf("✖ Error or credential not found during login process!");
                    continue;
                }
                return 0;
            }
            else if (strcmp(input, "2") == 0)
            {
                if (registerMenu(user, db) != 0)
                {
                    continue;
                }
                return 0;
            }
            else if (strcmp(input, "3") == 0)
            {
                system("clear");
                sqlite3_close(db);
                exit(0);
            }
        }
        system("clear");
        printf("✖ Invalid option!");
    }
    return 1;
}