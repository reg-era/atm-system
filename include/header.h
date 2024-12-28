#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Account
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};
// database function
int initDatabase(const char *url, sqlite3 **db);
int addUserDB(struct User *u, sqlite3 *db);
int loginUserDB(struct User *user, sqlite3 *db, const char *password);
int addAccountDB(int userID,struct Account *acc, sqlite3 *db);

// authentication functions
int loginMenu(struct User *user, sqlite3 *db);
int registerMenu(struct User *user);

// system function
void createNewAcc(struct User u, sqlite3 *db);
void mainMenu(struct User u, sqlite3 *db);
void checkAllAccounts(struct User u);

// utilitis function
void clearInputBuffer();
int validPhone(int phone);
int validAccountType(char *accountType);
int validDate(int month, int day, int year);
int validAccountNumber(int accountNbr);

#endif