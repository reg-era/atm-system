#include "header.h"
#include <sqlite3.h>

void mainMenu(User user, sqlite3 *db)
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
            clearInputBuffer();
            continue;
        }

        switch (option)
        {
        case 1:
            createNewAcc(user, db);
            break;
        case 2:
            printf("What is the account number you want to change: ");
            while (1)
            {
                int accNB;
                if (scanf("%d", &accNB) == 1)
                {
                    printf("Wich information do you want to update?\n1 --> phone number\n2 --> county\n");
                    while (1)
                    {
                        int upOption;
                        if (scanf("%d", &upOption) == 1 && (upOption == 1 || upOption == 2))
                        {
                            updatUserAcc(upOption, user, db, accNB);
                            break;
                        }
                        printf("Invalid option! try again : ");
                        clearInputBuffer();
                        continue;
                    }
                    break;
                }
                printf("Invalid option! Please enter account number: ");
                clearInputBuffer();
                continue;
            }
            break;
        case 3:
            printf("Enter account number: ");
            while (1)
            {
                int accNB;
                if (scanf("%d", &accNB) == 1)
                {
                    checkAcount(user, db, accNB);
                    system("clean");
                    break;
                }
                else
                {
                    printf("Invalid option! Please enter account number: ");
                    clearInputBuffer();
                    continue;
                }
            }
            break;
        case 4:
            checkAllAccounts(user, db);
            break;
        case 5:
            printf("Enter the account number of the customer: ");
            while (1)
            {
                int accNB;
                if (scanf("%d", &accNB) == 1)
                {
                    printf("Do you want to:\n\t1 --> Withdraw\n\t2 --> Deposit\n\nEnter your choice: ");
                    while (1)
                    {
                        int upOption;
                        if (scanf("%d", &upOption) == 1 && (upOption == 1 || upOption == 2))
                        {
                            makeTransaction(upOption, user, db, accNB);
                            break;
                        }
                        printf("Invalid option! try again : ");
                        clearInputBuffer();
                        continue;
                    }
                    break;
                }
                printf("Invalid option! Please enter account number: ");
                clearInputBuffer();
                continue;
            }
            break;
        case 6:
            printf("Enter account number: ");
            while (1)
            {
                int accNB;
                if (scanf("%d", &accNB) == 1)
                {
                    system("clean");
                    printf("Are you sure you want to remove the %d account\nType [yes] to confirm: ", accNB);
                    char decision[3];
                    if (scanf("%3s", decision) == 1 && strcmp(decision, "yes") == 0)
                        deletAccount(user, db, accNB);
                    break;
                    printf("Failed to remove the %d account", accNB);
                }
                printf("Invalid option! Please enter account number: ");
                clearInputBuffer();
            }
            break;
        case 7:
            printf("Enter the account number you want to transfere ownership: ");
            int accNB;
            if (scanf("%d", &accNB) == 1)
            {
                transferAccount(user, db, accNB);
                break;
            }
            printf("Invalid option! Please retry: ");
            clearInputBuffer();
            break;
        case 8:
            system("clear");
            sqlite3_close(db);
            exit(0);
        default:
            system("clear");
            printf("Invalid operation!\n");
        }
    }
}

void initMenu(User *user, sqlite3 *db)
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
            clearInputBuffer();
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

    User u;
    initMenu(&u, db);
    mainMenu(u, db);

    return 0;
};
