#include <termios.h>
#include "header.h"

int registerMenu(User *user)
{
    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUser Login: ");

    scanf("%s", user->name);

    printf("\n\n\t\tEnter the password to login: ");
    scanf("%s", user->password);

    return 0;
}

int loginMenu(User *user, sqlite3 *db)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t======= Bank Management System =======\n\n\n\t\tUsername: ");
    scanf("%s", user->name);

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return 1;
    }

    char password[100];
    printf("\n\n\t\tEnter the password to login: ");
    scanf("%s", password);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return 1;
    }

    return loginUserDB(user, db, password);
}