#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

typedef struct
{
    int month, day, year;
} Date;

// all fields for each record of an account
typedef struct
{
    int id;
    int userId;
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    Date deposit;
} Account;

typedef struct
{
    int id;
    char name[50];
    char password[50];
} User;

// database function
void initDatabase(const char *url, sqlite3 **db);
int addUserDB(User *u, sqlite3 *db);
int loginUserDB(User *user, sqlite3 *db, const char *password);
int addAccountDB(int userID, Account *acc, sqlite3 *db);
Account *getAllUserAcc(User *u, sqlite3 *db, int *count);
Account *getAccData(User u, sqlite3 *db, int accNB);
int deletAccount(User u, sqlite3 *db, int accNB);
void makeTransaction(int option, User u, sqlite3 *db, int accNB);
int transferAccount(User u, sqlite3 *db, int accNB);

// authentication functions
int loginMenu(User *user, sqlite3 *db);
int registerMenu(User *user, sqlite3 *db);

// system function
void createNewAcc(User u, sqlite3 *db);
int mainMenu(User u, sqlite3 *db);
void checkAllAccounts(User u, sqlite3 *db);
void updatUserAcc(int option, User u, sqlite3 *db, int accNB);
void checkAcount(User u, sqlite3 *db, int accNB);

// utilitis function
void success(User u, sqlite3 *db);
void stayOrReturn(void f(User u, sqlite3 *db), User u, sqlite3 *db);

void clearInputBuffer();
int validAccountType(char *accountType);
int validDate(int month, int day, int year);

#endif