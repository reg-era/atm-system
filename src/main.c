#include "header.h"

int main()
{
    sqlite3 *db = NULL;
    initDatabase("./data/data.db", &db);

    User user;

    if (initMenu(&user, db))
    {
        return 0;
    }

    mainMenu(user, db);

    return 0;
}