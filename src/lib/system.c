#include "header.h"

int createNewAcc(User u, sqlite3 *db)
{
    Account acc;
    char input[100];

    system("clear");
    printf("\t\t\t===== New record =====\n");

    int tryCount = 0;
    while (tryCount < 3)
    {
        printf("\nEnter today's date (dd/mm/yyyy): ");
        if (fgets(input, sizeof(input), stdin))
        {
            if (sscanf(input, "%d/%d/%d", &acc.deposit.day, &acc.deposit.month, &acc.deposit.year) == 3 &&
                validDate(acc.deposit.day, acc.deposit.month, acc.deposit.year))
                break;
            else
            {
                tryCount++;
                printf("✖ Invalid date\n");
            }
        }
        if (tryCount == 3)
            return 0;
    }

    tryCount = 0;
    while (tryCount < 3)
    {
        printf("\nEnter the account number (may not be taken): ");
        if (fgets(input, sizeof(input), stdin))
        {
            acc.accountNbr = strtol(input, NULL, 10);
            if (acc.accountNbr > 0)
            {
                if (getAccData(u.name, db, acc.accountNbr) != NULL)
                {
                    printf("✖ Account number exists\n");
                    tryCount++;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                printf("✖ Invalid account number\n");
            }
        }
        if (tryCount == 3)
            return 0;
    }

    tryCount = 0;
    while (tryCount < 3)
    {
        printf("\nEnter the country: ");
        if (fgets(input, sizeof(input), stdin))
        {
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0)
            {
                strncpy(acc.country, input, sizeof(acc.country) - 1);
                acc.country[sizeof(acc.country) - 1] = '\0';
                break;
            }
            else
            {
                printf("✖ Invalid country name\n");
            }
        }
        if (tryCount == 3)
            return 0;
    }

    tryCount = 0;
    while (tryCount < 3)
    {
        printf("\nEnter the phone number (10 digits): ");
        if (fgets(input, sizeof(input), stdin))
        {
            acc.phone = strtol(input, NULL, 10);
            if (acc.phone > 0 && acc.phone <= 9999999999)
            {
                break;
            }
            else
            {
                printf("✖ Invalid phone number\n");
            }
        }
        if (tryCount == 3)
            return 0;
    }

    tryCount = 0;
    while (tryCount < 3)
    {
        printf("\nEnter amount to deposit: $");
        if (fgets(input, sizeof(input), stdin))
        {
            acc.amount = strtod(input, NULL);
            if (acc.amount > 0)
            {
                break;
            }
            else
            {
                printf("✖ Invalid deposit amount\n");
            }
        }
        if (tryCount == 3)
            return 0;
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n");
    tryCount = 0;
    while (tryCount < 3)
    {
        printf(" Enter your choice: ");
        if (fgets(input, sizeof(input), stdin))
        {
            input[strcspn(input, "\n")] = 0;
            if (validAccountType(input))
            {
                strncpy(acc.accountType, input, sizeof(acc.accountType) - 1);
                acc.accountType[sizeof(acc.accountType) - 1] = '\0';
                break;
            }
            else
            {
                printf("✖ Invalid account type\n");
            }
        }
        if (tryCount == 3)
            return 0;
    }

    if (addAccountDB(u.id, &acc, db))
    {
        printf("\n✓ Account create succesfuly\n");
        finish(u, db);
        return 1;
    }
    return 0;
}

void makeTransaction(int option, User u, sqlite3 *db, int accNB)
{
    system("clear");
    Account *accData = getAccData(u.name, db, accNB);

    if (!strcmp(accData->accountType, "fixed01") || !strcmp(accData->accountType, "fixed02") || !strcmp(accData->accountType, "fixed03"))
    {
        printf("You cannot %s cash in fixed accounts!\n", option == 1 ? "withdraw" : "deposit");
        return;
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "UPDATE accounts SET balance = ? WHERE id = ? ;", -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return;
    }

    char input[100];
    double newAmount;

    printf("Enter the amount to %s: $", option == 1 ? "withdraw" : "deposit");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        newAmount = strtod(input, NULL);

        if (newAmount <= 0)
        {
            printf("✖ Invalid amount! Please enter a valid positive number.\n");
            sqlite3_finalize(stmt);
            return;
        }

        if (option == 1)
        {
            if (newAmount > accData->amount)
            {
                printf("✖ The amount you chose to withdraw is greater than your available balance!\n");
                sqlite3_finalize(stmt);
                return;
            }

            sqlite3_bind_double(stmt, 1, (accData->amount - newAmount));
        }
        else
        {
            sqlite3_bind_double(stmt, 1, (accData->amount + newAmount));
        }
    }
    else
    {
        printf("✖ Invalid input data.\n");
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 2, accData->id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    printf("\n✓ Transaction %s processed successfully\n", option == 1 ? "withdraw" : "deposit");
    finish(u, db);
}

int transferAccount(User u, sqlite3 *db, Account *accData)
{
    system("clear");
    printf("Which user do you want to transfer ownership to (user name): ");
    char name[50];

    if (fgets(name, sizeof(name), stdin))
    {
        system("clear");
        name[strcspn(name, "\n")] = 0;

        int id = getUserID(db, name);
        if (id < 0)
        {
            printf("✖ Error the user [ %s ] not found\n", name);
            return 1;
        }

        if (!addAccountDB(id, accData, db))
        {
            printf("✖ Error during transfer processe\n");
            return 1;
        }

        deletAccount(u, db, accData->accountNbr);
    }
    else
    {
        printf("\n✖ Invalid input\n");
        return 0;
    }

    return 0;
}
