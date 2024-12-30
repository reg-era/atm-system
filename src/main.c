#include "header.h"
#include <sqlite3.h>

void mainMenu(User user, sqlite3 *db)
{
    system("clear");
    while (1)
    {
        printf("\n\n\t\t======= ATM =======\n\n");
        printf("\n\t\t-->> Choose an option:\n");
        printf("\n\t\t[1]- Create a new account\n");
        printf("\n\t\t[2]- Update account information\n");
        printf("\n\t\t[3]- Check accounts\n");
        printf("\n\t\t[4]- List owned accounts\n");
        printf("\n\t\t[5]- Make Transaction\n");
        printf("\n\t\t[6]- Remove account\n");
        printf("\n\t\t[7]- Transfer ownership\n");
        printf("\n\t\t[8]- Exit\n");

        int option;
        if (scanf("%d", &option) != 1)
        {
            system("clear");
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }

        switch (option)
        {
        case 1:
            createNewAcc(user, db);
            break;

        case 2:
        {
            int accNB;
            printf("Enter the account number to update: ");
            if (scanf("%d", &accNB) != 1)
            {
                printf("Invalid account number!\n");
                clearInputBuffer();
                break;
            }

            printf("Choose information to update:\n1 --> Phone\n2 --> Country\n");
            int upOption;
            if (scanf("%d", &upOption) == 1 && (upOption == 1 || upOption == 2))
            {
                updatUserAcc(upOption, user, db, accNB);
            }
            else
            {
                printf("Invalid option!\n");
                clearInputBuffer();
            }
            break;
        }

        case 3:
        {
            int accNB;
            printf("Enter account number: ");
            if (scanf("%d", &accNB) == 1)
            {
                checkAcount(user, db, accNB);
            }
            else
            {
                printf("Invalid account number!\n");
                clearInputBuffer();
            }
            break;
        }

        case 4:
            checkAllAccounts(user, db);
            break;

        case 5:
        {
            int accNB;
            printf("Enter account number: ");
            if (scanf("%d", &accNB) == 1)
            {
                printf("Choose transaction:\n1 --> Withdraw\n2 --> Deposit\n");
                int transOption;
                if (scanf("%d", &transOption) == 1 && (transOption == 1 || transOption == 2))
                {
                    makeTransaction(transOption, user, db, accNB);
                }
                else
                {
                    printf("Invalid transaction option!\n");
                    clearInputBuffer();
                }
            }
            else
            {
                printf("Invalid account number!\n");
                clearInputBuffer();
            }
            break;
        }

        case 6:
        {
            int accNB;
            printf("Enter account number: ");
            if (scanf("%d", &accNB) == 1)
            {
                char decision[4];
                printf("Confirm deletion (yes): ");
                if (scanf("%3s", decision) == 1 && strcmp(decision, "yes") == 0)
                {
                    deletAccount(user, db, accNB);
                }
            }
            else
            {
                printf("Invalid account number!\n");
                clearInputBuffer();
            }
            break;
        }

        case 7:
        {
            int accNB;
            printf("Enter account number to transfer ownership: ");
            if (scanf("%d", &accNB) == 1)
            {
                transferAccount(user, db, accNB);
            }
            else
            {
                printf("Invalid account number!\n");
                clearInputBuffer();
            }
            break;
        }

        case 8:
            system("clear");
            sqlite3_close(db);
            exit(0);

        default:
            system("clear");
            printf("Invalid option!\n");
        }
    }
}

void initMenu(User *user, sqlite3 *db)
{
    system("clear");
    while (1)
    {
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Login/Register:\n");
        printf("\n\t\t[1]- Login\n");
        printf("\n\t\t[2]- Register\n");
        printf("\n\t\t[3]- Exit\n");

        int option;
        if (scanf("%d", &option) != 1)
        {
            system("clear");
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }

        switch (option)
        {
        case 1:
            if (!loginMenu(user, db))
                return;
            system("clear");
            printf("✖ Wrong username or password!");
            break;

        case 2:
            if (!registerMenu(user) && !addUserDB(user, db))
                return;
            system("clear");
            printf("✖ Registration failed try again!");
            break;

        case 3:
            system("clear");
            sqlite3_close(db);
            exit(0);

        default:
            system("clear");
            printf("✖ Invalid option!");
        }
    }
}

int main()
{
    sqlite3 *db = NULL;
    initDatabase("./data/data.db", &db);

    User user;
    initMenu(&user, db);
    mainMenu(user, db);

    return 0;
}