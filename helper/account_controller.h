#ifndef ACCOUNT_CONTROLLER_H
#define ACCOUNT_CONTROLLER_H


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../models/account_model.h"
#include "../models/db_information_model.h"
#include "../utils/constants.h"
#include "../utils/utils.h"

int getNewAccountId()
{
    int fd = open(dbInformationDatabase, O_RDONLY), id;
    DbInformationModel model;
    lockRecordDbInfo(fd, F_RDLCK);
    lseek(fd, 0, SEEK_SET);
    read(fd, &model, sizeof(model));

    id = model.accountCount + 1;

    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);
    return id;
}

int getAccountIdFromUserId(int userId, AccountType accType)
{
    int fd = open(accountDatabase, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("Error opening accounts file");
        return -1;
    }
    int accId = -1;

    AccountModel account;

    if (lockRecordDbInfo(fd, F_WRLCK) == -1)
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    lseek(fd, 0, SEEK_SET);

    int count = 0;
    while (read(fd, &account, sizeof(AccountModel)) != sizeof(AccountModel))
    {
        lockRecordAccountDb(fd, count, F_WRLCK);
        if (account.accType == accType && account.user_id == userId)
        {
            accId = account.account_id;
            lockRecordAccountDb(fd, count, F_UNLCK);
            break;
        }
        lockRecordAccountDb(fd, count, F_UNLCK);
        count++;
    }

    close(fd);
    return accId;
}

void createAccount(int user_id, AccountType accType)
{
    int fd = open(accountDatabase, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("Error opening accounts file");
        return;
    }
    int accId = getNewAccountId();

    AccountModel account;
    account.account_id = accId;
    account.user_id = user_id;
    account.accType = accType;
    account.balance = 0;

    // Lock the file for writing (ensuring no race conditions)
    if (lockRecordDbInfo(fd, F_WRLCK) == -1)
    {
        perror("Error locking file");
        close(fd);
        return;
    }

    // Write the account data
    lseek(fd, 0, SEEK_END); // Append to the end of the file
    if (write(fd, &account, sizeof(AccountModel)) != sizeof(AccountModel))
    {
        perror("Error writing to file");
    }
    else
    {
        printf("Savings account created for user_id %d\n", user_id);
    }

    // Unlock and close the file
    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);
}

void readAllAccounts()
{
    AccountModel account;
    int fd = open(accountDatabase, O_RDONLY, 0600);
    
    if (fd == -1)
    {
        perror("Error opening account database");
        return;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("Listing all accounts:\n");
    printf("\n%-10s %-10s %-15s %-10s\n", "AccountID", "UserID", "AccountType", "Balance");
    printf("----------------------------------------------\n");

    // Read the file record by record
    while (read(fd, &account, sizeof(AccountModel)) == sizeof(AccountModel))
    {
        if (account.user_id != -1)
        { // Only display records that are not marked as deleted (-1 user_id)

            printf("%-10d %-10d %-15s %-10d\n",
                   account.account_id,
                   account.user_id,
                   getAccountType(account.accType), // Convert enum to string
                   account.balance);
        }
    }

    close(fd);
}

#endif