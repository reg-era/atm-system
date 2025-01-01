#include <math.h>
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
            "\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
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

void checkAcount(User u, sqlite3 *db, int accNB)
{
    system("clear");
    Account *accountData = getAccData(u.name, db, accNB);

    if (accountData == NULL)
    {
        printf("✖ Error account not found\n");
        return;
    }

    printf(
        "\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
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