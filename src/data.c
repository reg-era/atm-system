#include <sqlite3.h>
#include <stdio.h>
#include "header.h"

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

int addUserDB(struct User *user, sqlite3 *db)
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, "INSERT INTO users (username, password) VALUES (?, ?)", -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, user->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user->password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 1;
    }

    user->id = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt);
    return 0;
}

int loginUserDB(struct User *user, sqlite3 *db, const char *password)
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, "SELECT id, username, password FROM users WHERE username = ?", -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, user->name, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user->id = sqlite3_column_int(stmt, 0);
        const char *usernameDB = (const char *)sqlite3_column_text(stmt, 1);
        const char *passwordDB = (const char *)sqlite3_column_text(stmt, 2);

        if (usernameDB == NULL || strcmp(usernameDB, user->name) == 1 ||
            passwordDB == NULL || strcmp(passwordDB, password) == 1)
        {
            sqlite3_finalize(stmt);
            return 1;
        }
        else if (strcmp(usernameDB, user->name) == 0 && strcmp(passwordDB, password) == 0)
        {
            strcpy(user->password, password);
            sqlite3_finalize(stmt);
            return 0;
        }
    }
    sqlite3_finalize(stmt);
    return 1;
}

int addAccountDB(int userID, struct Account *acc, sqlite3 *db)
{
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO accounts (created_at, account_number, account_type, country, phone, balance, user_id) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return 1;
    }

    char created_at[11];
    snprintf(created_at, sizeof(created_at), "%02d/%02d/%04d", acc->deposit.day, acc->deposit.month, acc->deposit.year);

    sqlite3_bind_text(stmt, 1, created_at, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, acc->accountNbr);
    sqlite3_bind_text(stmt, 3, acc->accountType, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, acc->country, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, acc->phone);
    sqlite3_bind_double(stmt, 6, acc->amount);
    sqlite3_bind_int(stmt, 7, userID);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}
