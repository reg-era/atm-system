#include "header.h"

void finish(User u, sqlite3 *db)
{
    char input[10];

invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");

    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "1") == 0)
        {
            system("clear");
            mainMenu(u, db);
        }
        else if (strcmp(input, "0") == 0)
        {
            sqlite3_close(db);
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

int validDate(int day, int month, int year, int inputLen)
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

    int finalLen = 7;
    finalLen += (day >= 10) ? 2 : 1;
    finalLen += (month >= 10) ? 2 : 1;

    if (inputLen != finalLen)
    {
        return 0;
    }

    return 1;
}

int validCountry(char *country)
{
    for (int i = 0; i < strlen(country); i++)
    {
        if (!(isalpha(country[i]) || country[i] == ' ' || country[i] == '-'))
        {
            return 0;
        }
    }
    return 1;
}

int validPhone(char *phone)
{
    for (int i = 0; i < strlen(phone); i++)
    {
        if (!(isdigit(phone[i]) || phone[i] == ' '))
        {
            return 0;
        }
    }
    return 1;
}

void getPassword(char *buffer, const char *prompt)
{
    struct termios oldt, newt;

    printf("%s", prompt);
    fflush(stdout);

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        buffer[0] = '\0';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = '\0';
    }
}
