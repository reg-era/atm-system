#ifndef PREPARE_H
#define PREPARE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sqlite3.h>
#include <termios.h>

typedef struct
{
    int month, day, year;
} Date;

// all fields for each record of an account
typedef struct
{
    int id;
    int userId;
    char country[64];
    char phone[64];
    char accountType[24];
    int accountNbr;
    double amount;
    Date deposit;
} Account;

typedef struct
{
    int id;
    char name[64];
    char password[64];
} User;

#endif
