#ifndef PREPARE_H
#define PREPARE_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <math.h>
#include <ctype.h>
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

#endif
