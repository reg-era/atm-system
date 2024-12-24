#include <sqlite3.h>
#include <stdio.h>

const char *_create_users_table =
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "username TEXT NOT NULL UNIQUE, "
    "password TEXT NOT NULL);";

const char *_create_accounts_table =
    "CREATE TABLE IF NOT EXISTS accounts ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "created_at TEXT NOT NULL, "
    "account_number INTEGER NOT NULL, "
    "account_type TEXT NOT NULL, "
    "country TEXT NOT NULL, "
    "phone TEXT NOT NULL, "
    "balance REAL NOT NULL, "
    "user_id INTEGER, "
    "FOREIGN KEY (user_id) REFERENCES users(id), "
    "CHECK(account_type IN ('current', 'savings', 'fixed01', 'fixed02', 'fixed03')) "
    ");";

int initDatabase(const char *url, sqlite3 **db)
{
    int err = sqlite3_open(url, db);
    if (err)
    {
        printf("Database can't open:\nError message: %s\n", sqlite3_errmsg(*db));
        return 1;
    }

    char *err_msg = 0;

    int status = sqlite3_exec(*db, "PRAGMA foreign_keys=ON;", 0, 0, &err_msg);
    if (status != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    status = sqlite3_exec(*db, _create_users_table, 0, 0, &err_msg);
    if (status != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    status = sqlite3_exec(*db, _create_accounts_table, 0, 0, &err_msg);
    if (status != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}