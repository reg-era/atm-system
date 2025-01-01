#include "header.h"

void finish(User u, sqlite3 *db)
{
    char input[10];
    int option;

    invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");

    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        option = strtol(input, NULL, 10);

        system("clear");

        if (option == 1)
        {
            mainMenu(u, db);
        }
        else if (option == 0)
        {
            system("clear");
            exit(0);
        }
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
}


void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int validAccountType(char *accountType)
{
    return (strcmp(accountType, "saving") == 0 ||
            strcmp(accountType, "current") == 0 ||
            strcmp(accountType, "fixed01") == 0 ||
            strcmp(accountType, "fixed02") == 0 ||
            strcmp(accountType, "fixed03") == 0);
}

int validDate(int day, int month, int year)
{
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2100)
    {
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return 0;
    }
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            if (day > 29)
                return 0;
        }
        else
        {
            if (day > 28)
                return 0;
        }
    }
    return 1;
}
