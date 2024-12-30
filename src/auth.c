#include <termios.h>
#include "header.h"

int registerMenu(User *user)
{
    struct termios oflags, nflags;
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");

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

    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", user->password);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return 1;
    }
    return 0;
}

int loginMenu(User *user, sqlite3 *db)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:\n");

    printf("\t\t\t\tUsername: ");
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
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login: ");
    scanf("%s", password);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return 1;
    }

    return loginUserDB(user, db, password);
}