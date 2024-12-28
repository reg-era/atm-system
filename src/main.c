#include "header.h"
#include <sqlite3.h>

void mainMenu(struct User user, sqlite3 *db)
{
    system("clear");
    int retry = 1;
    while (retry)
    {
        printf("\n\n\t\t======= ATM =======\n\n");
        printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
        printf("\n\t\t[1]- Create a new account\n");
        printf("\n\t\t[2]- Update account information\n");
        printf("\n\t\t[3]- Check accounts\n");
        printf("\n\t\t[4]- Check list of owned account\n");
        printf("\n\t\t[5]- Make Transaction\n");
        printf("\n\t\t[6]- Remove existing account\n");
        printf("\n\t\t[7]- Transfer ownership\n");
        printf("\n\t\t[8]- Exit\n");
        int option;

        if (scanf("%d", &option) != 1)
        {
            system("clear");
            printf("Invalid option! Please choose a valid operation.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (option)
        {
        case 1:
            createNewAcc(user, db);
            break;
        case 2:
            // student TODO : add your **Update account information** function
            // here
            break;
        case 3:
            // student TODO : add your **Check the details of existing accounts** function
            // here
            break;
        case 4:
            checkAllAccounts(user);
            break;
        case 5:
            // student TODO : add your **Make transaction** function
            // here
            break;
        case 6:
            // student TODO : add your **Remove existing account** function
            // here
            break;
        case 7:
            // student TODO : add your **Transfer owner** function
            // here
            break;
        case 8:
            system("clear");
            return;
        default:
            system("clear");
            printf("Invalid operation!\n");
        }
    }
}

void initMenu(struct User *user, sqlite3 *db)
{
    system("clear");
    int retry = 1;
    while (retry)
    {
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");
        int option;

        if (scanf("%d", &option) != 1)
        {
            system("clear");
            printf("Invalid option! Please choose a valid operation.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (option)
        {
        case 1:
            if (loginMenu(user, db))
            {
                system("clear");
                printf("\nWrong password or User Name.\n");
                retry = 1;
                continue;
            }
            retry = 0;
            break;

        case 2:
        {
            if (registerMenu(user))
            {
                system("clear");
                printf("Failed during registration. Try again.\n");
                retry = 1;
                continue;
            }
            if (addUserDB(user, db))
            {
                system("clear");
                printf("Failed to register user:\n%s\nTry again.\n", sqlite3_errmsg(db));
                retry = 1;
                continue;
            }
            retry = 0;
        }
        break;

        case 3:
            system("clear");
            return;
        default:
            system("clear");
            printf("Invalid option! Please choose a valid operation.\n");
            retry = 1;
        }
    }
}

int main()
{
    sqlite3 *db = NULL;
    if (initDatabase("./data/data.db", &db))
    {
        return 1;
    }

    struct User u;
    initMenu(&u, db);
    mainMenu(u, db);

    sqlite3_close(db);
    return 0;
};
