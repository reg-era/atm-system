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
struct Record
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
int addUserDB(struct User *u,sqlite3 *db);
int loginUserDB(struct User *user, sqlite3 *db, const char *password);


// authentication functions
int loginMenu(struct User *user, sqlite3 *db);
int registerMenu(struct User *user);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);

// utilitis function
int validName(char *name);
int validNumber(char *number); 
int initDatabase(const char *url, sqlite3 **db);

#endif