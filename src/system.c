#include "header.h"

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        // else if (option == 1)
        // mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        // mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        // mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u, sqlite3 *db)
{
    struct Account acc;

    system("clear");
    printf("\t\t\t===== New record =====\n");

    while (1)
    {
        printf("\nEnter today's date (mm/dd/yyyy): ");
        if (scanf("%d/%d/%d", &acc.deposit.month, &acc.deposit.day, &acc.deposit.year) == 3 && validDate(acc.deposit.month, acc.deposit.day, acc.deposit.year))
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

    if (addAccountDB(u.id,&acc, db))
    {
        success(u);
    }
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Account acc;

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (0)
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   acc.accountNbr,
                   acc.deposit.day,
                   acc.deposit.month,
                   acc.deposit.year,
                   acc.country,
                   acc.phone,
                   acc.amount,
                   acc.accountType);
        }
    }
    success(u);
}
