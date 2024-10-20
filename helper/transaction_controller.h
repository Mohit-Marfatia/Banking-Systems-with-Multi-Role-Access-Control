#ifndef TRANSACTION_CONTROLLER_H
#define TRANSACTION_CONTROLLER_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../models/transaction_model.h"
#include "../enums/account_type.h"
#include "./account_controller.h"
#include "../utils/constants.h"

int getNewTransactionId()
{
    int fd = open(dbInformationDatabase, O_RDONLY), id;
    DbInformationModel model;
    lockRecordDbInfo(fd, F_RDLCK);
    lseek(fd, 0, SEEK_SET);
    read(fd, &model, sizeof(model));

    id = model.transactionsCount + 1;

    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);
    return id;
}

void transactMoney(int fromUserId, int toUserId, int amount, AccountType accType, TransactionType transactionType) {
    int fromAccId = getAccountIdFromUserId(fromUserId, accType);
    int toAccId = getAccountIdFromUserId(toUserId, accType);
    int transactionId = getNewTransactionId();
    AccountModel account, fromAccModel, toAccModel;
    fromAccModel = getAccountModelFromAccountId(fromAccId);
    toAccModel = getAccountModelFromAccountId(toAccId);

    TransactionModel transaction;
    transaction.transaction_id = transactionId;
    transaction.account_id = fromAccId; // Account ID from which the transaction is made
    transaction.fromUserId = fromUserId; // Keep user ID for the transaction record
    transaction.toUserId = toUserId; // Only needed for transfer
    transaction.amount = amount;
    transaction.transactionType = transactionType;

    // Open the account database to read and write
    int fd = open(accountDatabase, O_RDWR);
    if (fd == -1) {
        perror("Error opening accounts database");
        return;
    }

    bool fromAccountFound = false;
    bool toAccountFound = false;

    // Lock the account database for reading
    lockAccountDb(fd, F_RDLCK);
    lseek(fd, 0, SEEK_SET);
    printf("---debug---\n fromAccId: %d\n", fromAccId);
    // Read through the accounts to find the relevant accounts
    while (read(fd, &account, sizeof(AccountModel)) == sizeof(AccountModel)) {
        // Check for the from account
        if (account.account_id == fromAccId) {
            fromAccountFound = true;

            if (transactionType == WITHDRAWAL) {
                // Check if sufficient balance is available
                if (account.balance >= amount) {
                    account.balance -= amount; // Deduct the amount for withdrawal
                    fromAccModel.account_id = account.account_id;
                    fromAccModel.user_id = account.user_id;
                    fromAccModel.balance = account.balance;
                    fromAccModel.accType = account.accType;
                } else {
                    printf("Insufficient balance for withdrawal.\n");
                    lockAccountDb(fd, F_UNLCK);
                    close(fd);
                    return; // Exit if not enough balance
                }
            } else if (transactionType == TRANSFER) {
                // Check if sufficient balance is available
                if (account.balance >= amount) {
                    account.balance -= amount; // Deduct the amount for transfer
                    fromAccModel.account_id = account.account_id;
                    fromAccModel.user_id = account.user_id;
                    fromAccModel.balance = account.balance;
                    fromAccModel.accType = account.accType;
                } else {
                    printf("Insufficient balance for transfer. balance: %d transfer: %d\n", account.balance, amount);
                    lockAccountDb(fd, F_UNLCK);
                    close(fd);
                    return; // Exit if not enough balance
                }
            }
        }

        // Check for the to account only if it's a transfer or deposit
        if (transactionType == TRANSFER && account.account_id == toAccId) {
            toAccountFound = true;
            account.balance += amount; // Add the amount to the to account
                    toAccModel.account_id = account.account_id;
                    toAccModel.user_id = account.user_id;
                    toAccModel.balance = account.balance;
                    toAccModel.accType = account.accType;
        } else if (transactionType == DEPOSIT && account.account_id == toAccId) {
            toAccountFound = true;
            account.balance += amount; // Add the amount to the deposit account
                    toAccModel.account_id = account.account_id;
                    toAccModel.user_id = account.user_id;
                    toAccModel.balance = account.balance;
                    toAccModel.accType = account.accType;
        }
    }

    // Unlock the account database
    lockAccountDb(fd, F_UNLCK);

    // If the transfer account is not found, print an error message
    if (transactionType == TRANSFER && !toAccountFound) {
        printf("Recipient account not found.\n");
        close(fd);
        return;
    }

    // Write the updated account records back to the database
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &account, sizeof(AccountModel)) == sizeof(AccountModel)) {
        if ((transactionType == WITHDRAWAL && account.account_id == fromAccId) || 
            (transactionType == TRANSFER && account.account_id == fromAccId)){
            lseek(fd, -sizeof(AccountModel), SEEK_CUR); // Move back to overwrite
            write(fd, &fromAccModel, sizeof(AccountModel));
        }
            if((transactionType == TRANSFER && account.account_id == toAccId) || 
            (transactionType == DEPOSIT && account.account_id == toAccId)) {
            lseek(fd, -sizeof(AccountModel), SEEK_CUR); // Move back to overwrite
            write(fd, &toAccModel, sizeof(AccountModel)); // Write the updated account back

            break; // Break after updating the necessary accounts
        }
    }

    // Log the transaction
    int logFd = open(transactionDatabase, O_WRONLY| O_CREAT, 0644);
    if (logFd == -1) {
        perror("Error opening transaction database");
        close(fd);
        return;
    }
    lseek(logFd, 0, SEEK_END);
    perror("error seeking:");
    // Write the transaction to the file
    if (write(logFd, &transaction, sizeof(TransactionModel)) != sizeof(TransactionModel)) {
        perror("Error writing transaction to database");
    }

    DbInformationModel model;
    int fd2 = open(dbInformationDatabase, O_RDWR);
    lockRecordDbInfo(fd2, F_WRLCK);
    if (fd2 < 0)
    {
        perror("Error opening user information database");
    }

    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        perror("Error seeking in user information database");
        close(fd2);
    }

    if (read(fd2, &model, sizeof(DbInformationModel)) != sizeof(DbInformationModel))
    {
        perror("Error reading from user information database");
        close(fd2);
    }

    model.transactionsCount++;

    // Write updated model back to the file
    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        perror("Error seeking to start of user information database");
        close(fd2);
    }

    if (write(fd2, &model, sizeof(DbInformationModel)) != sizeof(DbInformationModel))
    {
        perror("Error writing to user information database");
        close(fd2);
    }
    lockRecordDbInfo(fd2, F_UNLCK);
    close(fd2);


    close(logFd);
    close(fd);
}

void readAllTransaction() {
    TransactionModel transaction;
    int fd = open(transactionDatabase, O_RDONLY, 0600);
    if (fd == -1) {
        perror("Error opening transaction database");
        return;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);
    perror("seek error: ");
    printf("Listing all transactions:\n");
    printf("\n%-15s %-15s %-15s %-15s %-20s %-10s\n", "Transaction ID", "Account ID", "From User ID", "To User ID", "Transaction Type", "Amount");
    printf("-------------------------------------------------------------------------------\n");

    // Read the file record by record
    while (read(fd, &transaction, sizeof(TransactionModel)) == sizeof(TransactionModel)) {
        // perror("read error");
        // Display the transaction details
        // printf("%s", getTransactionType(transaction.transactionType));
        printf("%-15d %-15d %-15d %-15d %-20s %-10d\n",
               transaction.transaction_id,
               transaction.account_id,
               transaction.fromUserId,
               transaction.toUserId,
               getTransactionType(transaction.transactionType),
               transaction.amount);
    }

    close(fd);
}

int compareTransactionIds(const void *a, const void *b)
{
    TransactionModel *t1 = (TransactionModel *)a;
    TransactionModel *t2 = (TransactionModel *)b;

    // Reverse order: higher transaction_id comes first
    return t2->transaction_id - t1->transaction_id;
}

char *readTransactionsOfUserId(int userId)
{
    TransactionModel transaction;
    int fd = open(transactionDatabase, O_RDONLY, 0600);

    if (fd == -1)
    {
        perror("Error opening transaction database");
        return NULL;
    }

    // Lock the transaction database for reading
    if (lockTransactionDb(fd, F_RDLCK) == -1)
    {
        perror("Error locking transaction database");
        close(fd);
        return NULL;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    size_t initial_size = 512; // Initial size of the buffer
    size_t current_size = initial_size;
    size_t content_length = 0;

    // Dynamically allocate memory for the string
    char *str = (char *)malloc(initial_size * sizeof(char));
    if (str == NULL)
    {
        perror("Failed to allocate memory");
        lockTransactionDb(fd, F_UNLCK); // Unlock before returning
        close(fd);
        return NULL;
    }

    // Initialize the buffer
    str[0] = '\0'; // Set the string to be empty initially

    // Append header to the buffer
    content_length += snprintf(str + content_length, current_size - content_length,
                               "\n%-15s %-15s %-15s %-15s %-20s %-10s\n", 
                               "TransactionID", "AccountID", "FromUserID", "ToUserID", 
                               "TransactionType", "Amount");

    // Append the separator
    content_length += snprintf(str + content_length, current_size - content_length,
                               "--------------------------------------------------------------------------\n");

    // Prepare to store transactions in an array for sorting
    size_t transaction_count = 0;
    size_t transaction_capacity = 10; // Initial capacity
    TransactionModel *transactions = (TransactionModel *)malloc(transaction_capacity * sizeof(TransactionModel));
    if (transactions == NULL)
    {
        perror("Failed to allocate memory for transactions");
        free(str);
        lockTransactionDb(fd, F_UNLCK);// Unlock before returning
        close(fd);
        return NULL;
    }

    // Loop through the transactions and collect matching records
    while (read(fd, &transaction, sizeof(TransactionModel)) == sizeof(TransactionModel))
    {
        if (transaction.fromUserId == userId || transaction.toUserId == userId)
        {
            // Ensure there is enough space to store the new transaction
            if (transaction_count >= transaction_capacity)
            {
                transaction_capacity *= 2;
                transactions = (TransactionModel *)realloc(transactions, transaction_capacity * sizeof(TransactionModel));
                if (transactions == NULL)
                {
                    perror("Failed to reallocate memory for transactions");
                    free(str);
        lockTransactionDb(fd, F_UNLCK);// Unlock before returning
                    close(fd);
                    return NULL;
                }
            }
            transactions[transaction_count++] = transaction;
        }
    }

    // Unlock the transaction database
        lockTransactionDb(fd, F_UNLCK);
    close(fd);

    // Sort the transactions by transaction ID in reverse order
    qsort(transactions, transaction_count, sizeof(TransactionModel), compareTransactionIds);

    // Append sorted transactions to the buffer
    for (size_t i = 0; i < transaction_count; i++)
    {
        TransactionModel t = transactions[i];

        // Ensure there is enough space in the string buffer to append the new transaction data
        if (content_length + 100 >= current_size)
        {
            current_size *= 2; // Double the buffer size
            str = (char *)realloc(str, current_size * sizeof(char));
            if (str == NULL)
            {
                perror("Failed to reallocate memory");
                free(transactions);
                return NULL;
            }
        }

        // Append transaction details to the buffer
        content_length += snprintf(str + content_length, current_size - content_length,
                                   "%-15d %-15d %-15d %-15d %-20s %-10d\n",
                                   t.transaction_id,
                                   t.account_id,
                                   t.fromUserId,
                                   t.toUserId,
                                   getTransactionType(t.transactionType),  // Convert enum to string
                                   t.amount);
    }

    // Free the transactions array
    free(transactions);

    return str;
}

#endif