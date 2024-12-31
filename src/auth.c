#include <termios.h>
#include "header.h"

int registerMenu(User *user, sqlite3 *db)
{
    system("clear");

    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUser Login (20 character max): ");
    if (scanf("%s", user->name) != 1 || strlen(user->name) > 20)
        return 1;

    printf("\n\n\t\tEnter the password to login (30 character max): ");
    if (scanf("%s", user->password) != 1 || strlen(user->password) > 30)
        return 1;

    return addUserDB(user, db);
}

int loginMenu(User *user, sqlite3 *db)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUsername: ");
    if (scanf("%s", user->name) != 1 || strlen(user->name) > 20)
        return 1;

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return 1;
    }

    char password[50];
    printf("\n\n\t\tEnter the password to login: ");
    if (scanf("%s", password) != 1 || strlen(password) > 30)
        return 1;

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return 1;
    }

    return loginUserDB(user, db, password);
}