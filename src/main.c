#include "header.h"
#include <sqlite3.h>

int mainMenu(User user, sqlite3 *db)
{
    char input[10];

    printf(
        "\n\n\t\t======= ATM ======="
        "\n\n\n\t\t-->> Choose an option:"
        "\n\n\t\t[1]- Create a new account"
        "\n\n\t\t[2]- Update account information"
        "\n\n\t\t[3]- Check accounts"
        "\n\n\t\t[4]- List owned accounts"
        "\n\n\t\t[5]- Make Transaction"
        "\n\n\t\t[6]- Remove account"
        "\n\n\t\t[7]- Transfer ownership"
        "\n\n\t\t[8]- Exit"
        "\nEnter your option: ");

    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "1") == 0)
        {
            createNewAcc(user, db);
        }
        else if (strcmp(input, "2") == 0)
        {
            char accInput[10];
            printf("Enter the account number to update: ");
            if (fgets(accInput, sizeof(accInput), stdin))
            {
                accInput[strcspn(accInput, "\n")] = 0;
                int accNB = atoi(accInput);
                if (accNB > 0)
                {
                    printf("Choose information to update:\n1 --> Phone\n2 --> Country\n");
                    char upInput[10];
                    if (fgets(upInput, sizeof(upInput), stdin))
                    {
                        upInput[strcspn(upInput, "\n")] = 0;
                        if (strcmp(upInput, "1") == 0 || strcmp(upInput, "2") == 0)
                        {
                            int upOption = atoi(upInput);
                            updatUserAcc(upOption, user, db, accNB);
                        }
                        else
                        {
                            printf("Invalid option!\n");
                        }
                    }
                }
                else
                {
                    printf("Invalid account number!\n");
                }
            }
        }
        else if (strcmp(input, "3") == 0)
        {
            char accInput[10];
            printf("Enter account number: ");
            if (fgets(accInput, sizeof(accInput), stdin))
            {
                accInput[strcspn(accInput, "\n")] = 0;
                int accNB = atoi(accInput);
                if (accNB > 0)
                {
                    checkAcount(user, db, accNB);
                }
                else
                {
                    printf("Invalid account number!\n");
                }
            }
        }
        else if (strcmp(input, "4") == 0)
        {
            checkAllAccounts(user, db);
        }
        else if (strcmp(input, "5") == 0)
        {
            char accInput[10];
            printf("Enter account number: ");
            if (fgets(accInput, sizeof(accInput), stdin))
            {
                accInput[strcspn(accInput, "\n")] = 0;
                int accNB = atoi(accInput);
                if (accNB > 0)
                {
                    printf("Choose transaction:\n1 --> Withdraw\n2 --> Deposit\n");
                    char transInput[10];
                    if (fgets(transInput, sizeof(transInput), stdin))
                    {
                        transInput[strcspn(transInput, "\n")] = 0;
                        if (strcmp(transInput, "1") == 0 || strcmp(transInput, "2") == 0)
                        {
                            int transOption = atoi(transInput);
                            makeTransaction(transOption, user, db, accNB);
                        }
                        else
                        {
                            printf("Invalid transaction option!\n");
                        }
                    }
                }
                else
                {
                    printf("Invalid account number!\n");
                }
            }
        }
        else if (strcmp(input, "6") == 0)
        {
            char accInput[10];
            printf("Enter account number: ");
            if (fgets(accInput, sizeof(accInput), stdin))
            {
                accInput[strcspn(accInput, "\n")] = 0;
                int accNB = atoi(accInput);
                if (accNB > 0)
                {
                    char decision[10];
                    printf("Confirm deletion (yes): ");
                    if (fgets(decision, sizeof(decision), stdin))
                    {
                        decision[strcspn(decision, "\n")] = 0;
                        if (strcmp(decision, "yes") == 0)
                        {
                            deletAccount(user, db, accNB);
                        }
                        else
                        {
                            printf("Deletion canceled.\n");
                        }
                    }
                }
                else
                {
                    printf("Invalid account number!\n");
                }
            }
        }
        else if (strcmp(input, "7") == 0)
        {
            char accInput[10];
            printf("Enter account number to transfer ownership: ");
            if (fgets(accInput, sizeof(accInput), stdin))
            {
                accInput[strcspn(accInput, "\n")] = 0;
                int accNB = atoi(accInput);
                if (accNB > 0)
                {
                    transferAccount(user, db, accNB);
                }
                else
                {
                    printf("Invalid account number!\n");
                }
            }
        }
        else if (strcmp(input, "8") == 0)
        {
            system("clear");
            sqlite3_close(db);
            exit(0);
        }
        else
        {
            system("clear");
            printf("✖ Invalid option!\n");
        }
    }
    else
    {
        printf("Failed to read input.\n");
    }

    return 1;
}

int initMenu(User *user, sqlite3 *db)
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
                printf("✖ Error during login process!");
                return 1;
            }
            return 0;
        }
        else if (strcmp(input, "2") == 0)
        {
            if (registerMenu(user, db) != 0)
            {
                system("clear");
                printf("✖ Error during registration process!");
                return 1;
            }
            return 0;
        }
        else if (strcmp(input, "3") == 0)
        {
            sqlite3_close(db);
            exit(0);
        }
        else
        {
            system("clear");
            printf("✖ Invalid option!\n");
        }
    }
    return 1;
}

int main()
{
    system("clear");

    sqlite3 *db = NULL;
    initDatabase("./data/data.db", &db);

    User user;

    int err = 0;
    do
    {
        err = initMenu(&user, db);
        clearInputBuffer();
    } while (err != 0);

    system("clear");

    do
    {
        err = mainMenu(user, db);
        clearInputBuffer();
    } while (err != 0);

    return 0;
}