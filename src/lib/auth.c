#include "header.h"

int registerMenu(User *user, sqlite3 *db)
{
    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUser Login (20 character max): ");
    char nameInput[100];
    if (!fgets(nameInput, sizeof(nameInput), stdin))
    {
        system("clear");
        printf("✖ Error during login process!");
        return 1;
    }
    nameInput[strcspn(nameInput, "\n")] = 0;
    if (strlen(nameInput) > 20)
    {
        system("clear");
        printf("✖ Username exceeds 20 characters!\n");
        return 1;
    }
    strcpy(user->name, nameInput);

    printf("\n\n\t\tEnter the password to login (30 character max): ");
    char passwordInput[100];
    if (!fgets(passwordInput, sizeof(passwordInput), stdin))
    {
        system("clear");
        printf("✖ Error during login process!");
        return 1;
    }
    passwordInput[strcspn(passwordInput, "\n")] = 0;
    if (strlen(passwordInput) > 30)
    {
        system("clear");
        printf("✖ Password exceeds 30 characters!\n");
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

int loginMenu(User *user, sqlite3 *db) {
    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUsername: ");
    char input[100];
    
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) > 20) {
            return 1;
        }
        strcpy(user->name, input);
    } else {
        return 1;
    }

    char *password = getpass("\n\n\t\tEnter the password to login: ");
    if (password == NULL || strlen(password) > 30) {
        return 1;
    }

    return loginUserDB(user, db, password);
}