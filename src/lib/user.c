#include "header.h"

void checkAccount(User u, sqlite3 *db, int accNB)
{
    system("clear");
    Account *accountData = getAccData(u.name, db, accNB);

    if (accountData == NULL)
    {
        printf("✖ Error account not found\n");
        return;
    }

    printf(
        "\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
        accountData->accountNbr,
        accountData->deposit.day,
        accountData->deposit.month,
        accountData->deposit.year,
        accountData->country,
        accountData->phone,
        accountData->amount,
        accountData->accountType);

    if (strcmp(accountData->accountType, "current") == 0)
    {
        printf(" --> You will not get interests because the account is of type current");
    }
    else if (strcmp(accountData->accountType, "saving") == 0)
    {
        printf(
            " --> You will get $%.2f as interest on day %d of every month",
            round((accountData->amount * (0.07) * (1 / 12))), accountData->deposit.day);
    }
    else if (strcmp(accountData->accountType, "fixed01") == 0)
    {
        printf(
            " --> You will get $%.2f as interest on day %d/%d of every year",
            round(accountData->amount * 0.04), accountData->deposit.day, accountData->deposit.month);
    }
    else if (strcmp(accountData->accountType, "fixed02") == 0)
    {
        printf(
            " --> You will get $%.2f as interest on day %d/%d of every 2 year",
            round(accountData->amount * 0.05 * 2), accountData->deposit.day, accountData->deposit.month);
    }
    else if (strcmp(accountData->accountType, "fixed03") == 0)
    {
        printf(
            " --> You will get $%.2f as interest on day %d/%d of every 3 years",
            round(accountData->amount * 0.08 * 3), accountData->deposit.day, accountData->deposit.month);
    }

    printf("\n✓ Acount information displayed succesfuly\n");
    finish(u, db);
    return;
}

void checkAllAccounts(User u, sqlite3 *db)
{
    int count = 0;
    Account *allacount = getAllUserAcc(u.name, db, &count);

    if (allacount == NULL)
    {
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    for (int i = 0; i < count; i++)
    {
        printf("_____________________\n");
        printf(
            "\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
            allacount[i].accountNbr,
            allacount[i].deposit.day,
            allacount[i].deposit.month,
            allacount[i].deposit.year,
            allacount[i].country,
            allacount[i].phone,
            allacount[i].amount,
            allacount[i].accountType);
    }

    free(allacount);
    printf("\n✓ All account information displayed succesfuly\n");
    finish(u, db);
}

void updatUserAcc(int option, User u, sqlite3 *db, int accNB)
{
    system("clear");
    Account *accData = getAccData(u.name, db, accNB);

    sqlite3_stmt *stmt;
    const char *sql = option == 1 ? "UPDATE accounts SET phone = ? WHERE id = ? ;" : "UPDATE accounts SET country = ? WHERE id = ? ;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return;
    }

    while (1)
    {
        printf("Enter the new %s: ", (option == 1) ? "phone number" : "country name");
        if (option == 1)
        {
            char phone[50];
            if (fgets(phone, sizeof(phone), stdin))
            {
                phone[strcspn(phone, "\n")] = 0;
                if (strlen(phone) > 0)
                {
                    sqlite3_bind_text(stmt, 1, phone, -1, SQLITE_STATIC);
                    break;
                }
                else
                {
                    printf("✖ Invalid phone number! Please try again.\n");
                }
            }
        }
        else if (option == 2)
        {
            char country[50];
            if (fgets(country, sizeof(country), stdin))
            {
                country[strcspn(country, "\n")] = 0;
                if (strlen(country) > 0)
                {
                    sqlite3_bind_text(stmt, 1, country, -1, SQLITE_STATIC);
                    break;
                }
                else
                {
                    printf("✖ Invalid country name! Please try again.\n");
                }
            }
        }
        printf("Invalid option! Try again: ");
    }

    sqlite3_bind_int(stmt, 2, accData->id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    printf("\n✓ Account %s updated succesfuly\n", (option == 1) ? "phone number" : "country name");
    finish(u, db);
}