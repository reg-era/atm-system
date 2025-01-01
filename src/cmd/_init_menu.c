#include "header.h"

int mainMenu(User user, sqlite3 *db)
{
    system("clear");
    while (1)
    {
        char input[10];
        printf(
            "\n\n\t\t======= ATM ======="
            "\n\n\n\t\t-->> Choose an option:"
            "\n\n\t\t[1]- Create a new account"
            "\n\n\t\t[2]- Update account information"
            "\n\n\t\t[3]- Check accounts"
            "\n\n\t\t[4]- List owned accounts"
            "\n\n\t\t[5]- Make Transaction"
            "\n\n\t\t[6]- Remove account"
            "\n\n\t\t[7]- Transfer ownership"
            "\n\n\t\t[8]- Exit"
            "\n\nEnter your option: ");

        if (fgets(input, sizeof(input), stdin))
        {
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "1") == 0)
            {
                createNewAcc(user, db);
            }
            else if (strcmp(input, "2") == 0)
            {
                char accInput[10];
                printf("Enter the account number to update: ");
                if (fgets(accInput, sizeof(accInput), stdin))
                {
                    accInput[strcspn(accInput, "\n")] = 0;
                    int accNB = atoi(accInput);
                    if (accNB > 0 && getAccData(user.name, db, accNB) != NULL)
                    {
                        printf("Choose information to update:\n1 --> Phone\n2 --> Country\n");
                        char upInput[10];
                        if (fgets(upInput, sizeof(upInput), stdin))
                        {
                            upInput[strcspn(upInput, "\n")] = 0;
                            if (strcmp(upInput, "1") == 0 || strcmp(upInput, "2") == 0)
                            {
                                int upOption = atoi(upInput);
                                updatUserAcc(upOption, user, db, accNB);
                            }
                            else
                            {
                                system("clear");
                                printf("✖ Error invalid option!\n");
                            }
                        }
                    }
                    else
                    {
                        system("clear");
                        printf("✖ Error invalid account number!\n");
                    }
                }
            }
            else if (strcmp(input, "3") == 0)
            {
                char accInput[10];
                printf("Enter account number: ");
                if (fgets(accInput, sizeof(accInput), stdin))
                {
                    accInput[strcspn(accInput, "\n")] = 0;
                    int accNB = atoi(accInput);
                    if (accNB > 0)
                    {
                        checkAccount(user, db, accNB);
                    }
                    else
                    {
                        system("clear");
                        printf("✖ Error invalid account number!\n");
                    }
                }
            }
            else if (strcmp(input, "4") == 0)
            {
                checkAllAccounts(user, db);
            }
            else if (strcmp(input, "5") == 0)
            {
                char accInput[10];
                printf("Enter account number: ");
                if (fgets(accInput, sizeof(accInput), stdin))
                {
                    accInput[strcspn(accInput, "\n")] = 0;
                    int accNB = atoi(accInput);
                    if (accNB > 0 && getAccData(user.name, db, accNB) != NULL)
                    {
                        printf("Choose transaction:\n1 --> Withdraw\n2 --> Deposit\n");
                        char transInput[10];
                        if (fgets(transInput, sizeof(transInput), stdin))
                        {
                            transInput[strcspn(transInput, "\n")] = 0;
                            if (strcmp(transInput, "1") == 0 || strcmp(transInput, "2") == 0)
                            {
                                int transOption = atoi(transInput);
                                makeTransaction(transOption, user, db, accNB);
                            }
                            else
                            {
                                system("clear");
                                printf("✖ Error invalid transaction option!\n");
                            }
                        }
                    }
                    else
                    {
                        system("clear");
                        printf("✖ Error account not found!\n");
                    }
                }
            }
            else if (strcmp(input, "6") == 0)
            {
                char accInput[10];
                printf("Enter account number: ");
                if (fgets(accInput, sizeof(accInput), stdin))
                {
                    accInput[strcspn(accInput, "\n")] = 0;
                    int accNB = atoi(accInput);
                    Account *userAccount = getAccData(user.name, db, accNB);
                    if (accNB > 0 && userAccount != NULL)
                    {
                        char decision[10];
                        printf("Confirm deletion (yes): ");
                        if (fgets(decision, sizeof(decision), stdin))
                        {
                            decision[strcspn(decision, "\n")] = 0;
                            if (strcmp(decision, "yes") == 0)
                            {
                                if (deletAccount(user, db, accNB))
                                {
                                    system("clear");
                                    printf("✖ Error failed account deletion\n");
                                }
                                else
                                {
                                    system("clear");
                                    printf(
                                        "\n======= Deleted Account =======\n\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n"
                                        "\n✓ Account deleted succesfuly\n",
                                        userAccount->accountNbr,
                                        userAccount->deposit.day,
                                        userAccount->deposit.month,
                                        userAccount->deposit.year,
                                        userAccount->country,
                                        userAccount->phone,
                                        userAccount->amount,
                                        userAccount->accountType);
                                    finish(user, db);
                                }
                            }
                            else
                            {
                                system("clear");
                                printf("✖ Deletion canceled\n");
                            }
                        }
                    }
                    else
                    {
                        system("clear");
                        printf("✖ Error invalid account number!\n");
                    }
                }
            }
            else if (strcmp(input, "7") == 0)
            {
                char accInput[10];
                printf("Enter account number to transfer ownership: ");
                if (fgets(accInput, sizeof(accInput), stdin))
                {
                    accInput[strcspn(accInput, "\n")] = 0;
                    int accNB = atoi(accInput);
                    Account *userAccount = getAccData(user.name, db, accNB);
                    if (accNB > 0 && userAccount != NULL)
                    {
                        if (transferAccount(user, db, userAccount))
                        {
                            continue;
                        }
                        else
                        {
                            system("clear");
                            printf(
                                "\n======= Transfered account =======\n\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n"
                                "\n✓ Account transfered succesfuly\n",
                                userAccount->accountNbr,
                                userAccount->deposit.day,
                                userAccount->deposit.month,
                                userAccount->deposit.year,
                                userAccount->country,
                                userAccount->phone,
                                userAccount->amount,
                                userAccount->accountType);
                            finish(user, db);
                        }
                    }
                    else
                    {
                        system("clear");
                        printf("✖ Error invalid account number!\n");
                    }
                }
            }
            else if (strcmp(input, "8") == 0)
            {
                system("clear");
                sqlite3_close(db);
                exit(0);
            }
            else
            {
                system("clear");
                printf("✖ Invalid option!");
            }
        }
    }
    return 1;
}