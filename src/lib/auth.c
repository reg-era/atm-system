#include "header.h"

int registerMenu(User *user, sqlite3 *db)
{
    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUser Login (20 character max): ");
    char nameInput[64];
    if (!fgets(nameInput, sizeof(nameInput), stdin))
    {
        system("clear");
        printf("✖ Error during login process!");
        return 1;
    }
    nameInput[strcspn(nameInput, "\n")] = 0;
    if (strlen(nameInput) > 20 || strlen(nameInput) == 0)
    {
        system("clear");
        printf("✖ Invalid username\n");
        return 1;
    }
    strcpy(user->name, nameInput);

    printf("\n\n\t\tEnter the password to login (30 character max): ");
    char passwordInput[64];
    if (!fgets(passwordInput, sizeof(passwordInput), stdin))
    {
        system("clear");
        printf("✖ Error during login process!");
        return 1;
    }
    passwordInput[strcspn(passwordInput, "\n")] = 0;
    if (strlen(passwordInput) > 30 && strlen(passwordInput) == 0)
    {
        system("clear");
        printf("✖ Invalid password\n");
        return 1;
    }
    strcpy(user->password, passwordInput);
    if (addUserDB(user, db))
    {
        system("clear");
        printf("✖ Error or user already exists registration process!");
        return 1;
    }
    return 0;
}

int loginMenu(User *user, sqlite3 *db)
{
    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUsername: ");
    char input[64];

    if (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) > 20)
        {
            return 1;
        }
        strcpy(user->name, input);
    }
    else
    {
        return 1;
    }

    char password[32]; 
    getPassword(password, "\n\n\t\tEnter the password to login: ");

    // char *password = getpass("\n\n\t\tEnter the password to login: ");
    // if (password == NULL || strlen(password) > 30)
    // {
    //     return 1;
    // }

    int valid = loginUserDB(user, db, password);

    return valid;
}