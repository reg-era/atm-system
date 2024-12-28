#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void clearInputBuffer() {
    while (getchar() != '\n');  // Read and discard characters until the newline
}

int validPhone(int phone) {
    return phone >= 1000000000 && phone <= 9999999999;
}

int validAccountType(char *accountType) {
    return (strcmp(accountType, "saving") == 0 || 
            strcmp(accountType, "current") == 0 || 
            strcmp(accountType, "fixed01") == 0 || 
            strcmp(accountType, "fixed02") == 0 || 
            strcmp(accountType, "fixed03") == 0);
}

int validDate(int month, int day, int year) {
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2100) {
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return 0; 
    }
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) return 0;
        } else {
            if (day > 28) return 0;
        }
    }
    return 1; 
}

int validAccountNumber(int accountNbr) {
    return accountNbr > 0;
}


