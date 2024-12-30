#include <math.h>
#include "header.h"

void createNewAcc(User u, sqlite3 *db)
{
    Account acc;

    system("clear");
    printf("\t\t\t===== New record =====\n");

    while (1)
    {
        printf("\nEnter today's date (dd/mm/yyyy): ");
        if (scanf("%d/%d/%d", &acc.deposit.day, &acc.deposit.month, &acc.deposit.year) == 3 && validDate(acc.deposit.day, acc.deposit.month, acc.deposit.year))
            break;
        clearInputBuffer();
        printf("✖ Invalid date");
    }

    while (1)
    {
        printf("\nEnter the account number: ");
        if (scanf("%d", &acc.accountNbr) == 1 && validAccountNumber(acc.accountNbr))
            break;
        clearInputBuffer();
        printf("✖ Invalid number");
    }

    while (1)
    {
        printf("\nEnter the country: ");
        if (scanf("%s", acc.country) == 1 && strlen(acc.country) != 0)
            break;
        clearInputBuffer();
        printf("✖ Invalid country name");
    }

    while (1)
    {
        printf("\nEnter the phone number (10 digits): ");
        if (scanf("%d", &acc.phone) == 1 && validPhone(acc.phone))
            break;
        clearInputBuffer();
        printf("✖ Invalid phone number");
    }

    while (1)
    {
        printf("\nEnter amount to deposit: $");
        if (scanf("%lf", &acc.amount) == 1 && acc.amount > 0)
            break;
        clearInputBuffer();
        printf("✖ Invalid deposit amount");
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n");
    while (1)
    {
        printf(" Enter your choice: ");
        if (scanf("%s", acc.accountType) == 1 && validAccountType(acc.accountType))
            break;
        clearInputBuffer();
        printf("✖ Invalid account type");
    }

    if (addAccountDB(u.id, &acc, db))
    {
        success(u, db);
    }
    else
    {
        system("clear");
        printf("%s", sqlite3_errmsg(db));
        stayOrReturn(&createNewAcc, u, db);
    }
}

void checkAllAccounts(User u, sqlite3 *db)
{
    int count = 0;
    Account *allacount = getAllUserAcc(&u, db, &count);

    if (allacount == NULL)
    {
        stayOrReturn(&createNewAcc, u, db);
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
    success(u, db);
}

void checkAcount(User u, sqlite3 *db, int accNB)
{
    system("clear");
    Account *accountData = getAccData(u, db, accNB);

    if (accountData == NULL)
    {
        printf("Account not found\n");
        stayOrReturn(&mainMenu, u, db);
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

    success(u, db);
    return;
}