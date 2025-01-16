#ifndef HEADER_H
#define HEADER_H

#include "prepare.h"

// Main function
int mainMenu(User user, sqlite3 *db);
int initMenu(User *user, sqlite3 *db);

// Initializes the SQLite database connection.
void initDatabase(const char *url, sqlite3 **db);

// database function
int addUserDB(User *u, sqlite3 *db);
int loginUserDB(User *user, sqlite3 *db, char *password);
int addAccountDB(int userID, Account *acc, sqlite3 *db);
Account *getAllUserAcc(char *username, sqlite3 *db, int *count);
Account *getAccData(char *username, sqlite3 *db, int accNB);
int deletAccount(User u, sqlite3 *db, int accNB);
int getUserID(sqlite3 *db, char *name);

// User functions
void checkAccount(User u, sqlite3 *db, int accNB);
void checkAllAccounts(User u, sqlite3 *db);
void updatUserAcc(int option, User u, sqlite3 *db, int accNB);

// authentication functions
int loginMenu(User *user, sqlite3 *db);
int registerMenu(User *user, sqlite3 *db);

// system function
int createNewAcc(User u, sqlite3 *db);
void makeTransaction(int option, User u, sqlite3 *db, int accNB);
int transferAccount(User u, sqlite3 *db, Account *accData);

// utility functions
void finish(User u, sqlite3 *db);
int validDate(int month, int day, int year);
int validCountry(char *country);
int validPhone(char *phone);
#endif