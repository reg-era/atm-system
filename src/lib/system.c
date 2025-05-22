#include "header.h"

int createNewAcc(User u, sqlite3 *db)
{
    system("clear");
    printf("\t\t\t===== New record =====\n");
    Account acc;
    char input[100];

    printf("\nEnter today's date (dd/mm/yyyy): ");

    if (fgets(input, sizeof(input), stdin))
    {
        if (sscanf(input, "%d/%d/%d", &acc.deposit.day, &acc.deposit.month, &acc.deposit.year) != 3 ||
            !validDate(acc.deposit.day, acc.deposit.month, acc.deposit.year, strlen(input)))
        {
            system("clear");
            printf("✖ Invalid date");
            return 0;
        }
    }
    else
    {
        system("clear");
        printf("✖ Error on reading data input");
        return 0;
    }

    printf("\nEnter the account number (may not be taken): ");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;

        char *endptr;
        acc.accountNbr = strtol(input, &endptr, 10);
        if (*endptr == '\0' && acc.accountNbr > 0)
        {
            Account *accTest = getAccData(u.name, db, acc.accountNbr);
            if (accTest != NULL)
            {
                free(accTest);
                system("clear");
                printf("✖ Account number exists");
                return 0;
            }
            free(accTest);
        }
        else
        {
            system("clear");
            printf("✖ Invalid account number! Please enter a valid number");
            return 0;
        }
    }
    else
    {
        system("clear");
        printf("✖ Error on reading data input");
        return 0;
    }

    printf("\nEnter the country: ");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) > 0 && validCountry(input))
        {
            strncpy(acc.country, input, sizeof(acc.country) - 1);
            acc.country[sizeof(acc.country) - 1] = '\0';
        }
        else
        {
            system("clear");
            printf("✖ Invalid country name");
            return 0;
        }
    }
    else
    {
        system("clear");
        printf("✖ Error on reading data input");
        return 0;
    }

    printf("\nEnter the phone number: ");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) > 0 && validPhone(input))
        {
            strncpy(acc.phone, input, sizeof(acc.phone) - 1);
            acc.phone[sizeof(acc.phone) - 1] = '\0';
        }
        else
        {
            system("clear");
            printf("✖ Invalid phone number");
            return 0;
        }
    }
    else
    {
        system("clear");
        printf("✖ Error on reading data input");
        return 0;
    }

    printf("\nEnter amount to deposit: $");
    if (fgets(input, sizeof(input), stdin))
    {
        if ((strstr(input, ".") && strlen(input) > 14) || (!strstr(input, ".") && strlen(input) > 11))
        {
            system("clear");
            printf("✖ Maximume amount of transaction is 9999999999.99\nand make sure to use the form .99");
            return 0;
        }
        input[strcspn(input, "\n")] = 0;

        char *endptr;
        acc.amount = strtod(input, &endptr);
        if (*endptr != '\0' || acc.amount < 0)
        {
            system("clear");
            printf("✖ Invalid amount! Please enter a valid positive number");
            return 0;
        }
    }
    else
    {
        system("clear");
        printf("✖ Error on reading data input");
        return 0;
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\nEnter your choice: ");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "saving") == 0 ||
            strcmp(input, "current") == 0 ||
            strcmp(input, "fixed01") == 0 ||
            strcmp(input, "fixed02") == 0 ||
            strcmp(input, "fixed03") == 0)
        {
            strncpy(acc.accountType, input, sizeof(acc.accountType) - 1);
            acc.accountType[sizeof(acc.accountType) - 1] = '\0';
        }
        else
        {
            system("clear");
            printf("✖ Invalid account type");
            return 0;
        }
    }
    else
    {
        system("clear");
        printf("✖ Error on reading data input");
        return 0;
    }

    if (addAccountDB(u.id, &acc, db))
    {
        printf("\n✓ Account create succesfuly\n");
        finish(u, db);
        return 1;
    }
    system("clear");
    printf("✖ Error on account creation");
    return 0;
}

void makeTransaction(int option, User u, sqlite3 *db, int accNB)
{
    system("clear");
    Account *accData = getAccData(u.name, db, accNB);

    if (!strcmp(accData->accountType, "fixed01") || !strcmp(accData->accountType, "fixed02") || !strcmp(accData->accountType, "fixed03"))
    {
        free(accData);
        printf("You cannot %s cash in fixed accounts!\n", option == 1 ? "withdraw" : "deposit");
        return;
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "UPDATE accounts SET balance = ? WHERE id = ? ;", -1, &stmt, 0) != SQLITE_OK)
    {
        free(accData);
        sqlite3_finalize(stmt);
        return;
    }

    char input[100];
    double newAmount;

    printf("Enter the amount to %s: $", option == 1 ? "withdraw" : "deposit");
    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        if ((strstr(input, ".") && strlen(input) > 13) || (!strstr(input, ".") && strlen(input) > 10))
        {
            free(accData);
            sqlite3_finalize(stmt);
            system("clear");
            printf("✖ Maximume amount of transaction is 9999999999.99\nand make sure to use the form .99");
            return;
        }
        newAmount = strtod(input, NULL);

        if (newAmount <= 0)
        {
            free(accData);
            sqlite3_finalize(stmt);
            printf("✖ Invalid amount! Please enter a valid positive number.\n");
            return;
        }

        if (option == 1)
        {
            if (newAmount > accData->amount)
            {
                free(accData);
                sqlite3_finalize(stmt);
                printf("✖ The amount you chose to withdraw is greater than your available balance!\n");
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
        free(accData);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 2, accData->id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        free(accData);
        sqlite3_finalize(stmt);
        return;
    }

    free(accData);
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
