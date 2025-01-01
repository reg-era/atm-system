#include "header.h"

const char *_create_tables =
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "username TEXT NOT NULL UNIQUE, "
    "password TEXT NOT NULL); "
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
    "CHECK(account_type IN ('current', 'saving', 'fixed01', 'fixed02', 'fixed03')) "
    "UNIQUE(account_number, user_id) "
    ");";

void initDatabase(const char *url, sqlite3 **db)
{
    int err = sqlite3_open(url, db);
    if (err)
    {
        printf("Database can't open:\nError message: %s\n", sqlite3_errmsg(*db));
        exit(1);
    }

    char *err_msg = 0;

    int status = sqlite3_exec(*db, "PRAGMA foreign_keys=ON;", 0, 0, &err_msg);
    if (status != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }

    status = sqlite3_exec(*db, _create_tables, 0, 0, &err_msg);
    if (status != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
}

int addUserDB(User *user, sqlite3 *db)
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

int loginUserDB(User *user, sqlite3 *db, const char *password)
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

int addAccountDB(int userID, Account *acc, sqlite3 *db)
{
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT INTO accounts (created_at, account_number, account_type, country, phone, balance, user_id) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return 0;
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
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

Account *getAllUserAcc(char *username, sqlite3 *db, int *count)
{
    Account *accounts = NULL;
    sqlite3_stmt *stmt;
    const char *sql =
        "SELECT accounts.id, users.id, accounts.account_number, accounts.account_type, accounts.country, "
        "accounts.phone, accounts.balance, accounts.created_at FROM accounts "
        "JOIN users ON accounts.user_id = users.id "
        "WHERE users.username = ? ;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return NULL;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int capacity = 3;

    accounts = malloc(capacity * sizeof(Account));
    if (!accounts)
    {
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        if (i == capacity)
        {
            capacity *= 2;
            accounts = realloc(accounts, capacity * sizeof(Account));
            if (!accounts)
            {
                sqlite3_finalize(stmt);
                return NULL;
            }
        }

        accounts[i].id = sqlite3_column_int(stmt, 0);
        accounts[i].userId = sqlite3_column_int(stmt, 1);
        accounts[i].accountNbr = sqlite3_column_int(stmt, 2);
        strncpy(accounts[i].accountType, (const char *)sqlite3_column_text(stmt, 3), sizeof(accounts[i].accountType));
        strncpy(accounts[i].country, (const char *)sqlite3_column_text(stmt, 4), sizeof(accounts[i].country));
        accounts[i].phone = sqlite3_column_int(stmt, 5);
        accounts[i].amount = sqlite3_column_double(stmt, 6);

        const char *dateStr = (const char *)sqlite3_column_text(stmt, 7);
        sscanf(dateStr, "%d/%d/%d", &accounts[i].deposit.day, &accounts[i].deposit.month, &accounts[i].deposit.year);

        i++;
    }

    sqlite3_finalize(stmt);

    *count = i;
    return accounts;
}

Account *getAccData(char *username, sqlite3 *db, int accNB)
{
    int count = 0;
    Account *allacount = getAllUserAcc(username, db, &count);

    if (count == 0)
        return NULL;

    for (int i = 0; i < count; i++)
    {
        if (allacount[i].accountNbr == accNB)
        {
            return &allacount[i];
        }
    }

    return NULL;
}



int deletAccount(User u, sqlite3 *db, int accNB)
{
    Account *acc = getAccData(u.name, db, accNB);

    if (acc == NULL)
    {
        return 1;
    }

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM accounts WHERE account_number = ? AND user_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        return 1;
    }

    sqlite3_bind_int(stmt, 1, accNB);
    sqlite3_bind_int(stmt, 2, acc->userId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to delete account: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}



int getUserID(sqlite3 *db, char *name)
{
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "SELECT id FROM users WHERE username = ? ;", -1, &stmt, 0) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    int userID = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        userID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return userID;
}

