#ifndef ACCOUNT_CONTROLLER_H
#define ACCOUNT_CONTROLLER_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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
    int fd = open(accountDatabase, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening accounts file");
        return -1;
    }

    AccountModel account;
    int accId = -2;
    // Lock the account file
    if (lockRecordAccountDb(fd, F_RDLCK) == -1)
    {
        perror("Error locking accounts file");
        close(fd);  // Always close the file if an error occurs
        return -1;
    }

    // Move to the beginning of the file
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("Error seeking accounts file");
        lockRecordAccountDb(fd, F_UNLCK);
        close(fd);  // Always close the file if an error occurs
        return -1;
    }

    // Read through the file record by record
    while (read(fd, &account, sizeof(AccountModel)) == sizeof(AccountModel))
    {
        if (account.accType == accType && account.user_id == userId)
        {
            accId = account.account_id;
            break;
        }
    }

    // Unlock the file and close it
    if (lockRecordAccountDb(fd, F_UNLCK) == -1)
    {
        perror("Error unlocking accounts file");
    }

    close(fd);  // Close the file at the end
    return accId;
}

char *readAccountsOfUserId(int userId)
{
    AccountModel account;
    int fd = open(accountDatabase, O_RDONLY, 0600);

    if (fd == -1)
    {
        perror("Error opening account database");
        return NULL;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    size_t initial_size = 512; // Initial size of the buffer
    size_t current_size = initial_size;
    size_t content_length = 0;

    // Dynamically allocate memory for the string
    char *str = (char *)malloc(initial_size * sizeof(char));
    if (str == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Initialize the buffer
    str[0] = '\0'; // Set the string to be empty initially

    // Append header to the buffer
    content_length += snprintf(str + content_length, current_size - content_length,
                               "\n%-10s %-10s %-15s %-10s\n", "AccountID", "UserID", "AccountType", "Balance");
    
    // Append the separator
    content_length += snprintf(str + content_length, current_size - content_length,
                               "----------------------------------------------\n");

    // Loop through the accounts and append each matching account to the buffer
    while (read(fd, &account, sizeof(AccountModel)) == sizeof(AccountModel)) {
        if (account.user_id == userId) { // Only display relevant records

            // Ensure there is enough space to append the new account data
            if (content_length + 100 >= current_size) {
                current_size *= 2; // Double the buffer size
                str = (char *)realloc(str, current_size * sizeof(char));
                if (str == NULL) {
                    perror("Failed to reallocate memory");
                    return NULL;
                }
            }

            // Append account details to the buffer
            content_length += snprintf(str + content_length, current_size - content_length,
                                       "%-10d %-10d %-15s %-10d\n",
                                       account.account_id,
                                       account.user_id,
                                       getAccountType(account.accType), // Convert enum to string
                                       account.balance);
        }
    }

    close(fd);
    return str;
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