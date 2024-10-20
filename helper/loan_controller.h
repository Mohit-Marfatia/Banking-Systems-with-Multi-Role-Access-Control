#ifndef LOAN_CONTROLLER_H
#define LOAN_CONTROLLER_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../models/loan_model.h"
#include "../enums/loan_status.h"
#include "./account_controller.h"
#include "../utils/constants.h"

int getNewLoanId()
{
    int fd = open(dbInformationDatabase, O_RDONLY), id;
    DbInformationModel model;
    lockRecordDbInfo(fd, F_RDLCK);
    lseek(fd, 0, SEEK_SET);
    read(fd, &model, sizeof(model));

    id = model.loanCount + 1;

    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);
    return id;
}


int applyForLoan(int user_id, int loan_amount) {
    LoanModel loan;
    int fd = open(loanDatabase, O_WRONLY | O_APPEND | O_CREAT, 0600);
    if (fd == -1) {
        perror("Error opening loan database");
        return -1;
    }

    // Lock the loan database for writing
    if (lockTransactionDb(fd, F_WRLCK) == -1) {
        perror("Error locking loan database");
        close(fd);
        return -1;
    }

    // Populate loan details
    loan.loan_id = getNewLoanId(); // Ideally, this should be a unique ID generator
    loan.user_id = user_id;
    loan.loanStatus = PENDING;
    loan.loan_amount = loan_amount;
    loan.employee_assigned = -1; // Not assigned yet

    // Write the loan data to the loan database
    if (write(fd, &loan, sizeof(LoanModel)) == -1) {
        perror("Error writing to loan database");
        lockTransactionDb(fd, F_UNLCK);
        close(fd);
        return -1;
    }

    // Unlock and close the file
        lockTransactionDb(fd, F_UNLCK);
    close(fd);
    
    printf("Loan applied successfully.\n");
    return 0;
}

int addLoanAccount(int user_id, int loan_amount){
    AccountModel account;
    int fd = open(accountDatabase, O_WRONLY | O_APPEND | O_CREAT, 0600);
    if (fd == -1) {
        perror("Error opening account database");
        return -1;
    }

    if (lockAccountDb(fd, F_WRLCK) == -1) {
        perror("Error locking account database");
        close(fd);
        return -1;
    }

    // Populate account details for the loan
    account.account_id = getNewAccountId();
    account.user_id = user_id;
    account.accType = LOAN;
    account.balance = -1 * loan_amount;

    // Write the account data to the account database
    if (write(fd, &account, sizeof(AccountModel)) == -1) {
        perror("Error writing to account database");
        lockAccountDb(fd, F_UNLCK);
        close(fd);
        return -1;
    }

        lockAccountDb(fd, F_UNLCK);
    close(fd);

}

void readAllLoans() {
    LoanModel loan;
    int fd = open(loanDatabase, O_RDONLY, 0600); // Assuming loanDatabase.db is the filename
    if (fd == -1) {
        perror("Error opening loan database");
        return;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);
    perror("seek error: ");
    printf("Listing all loans:\n");
    printf("\n%-10s %-10s %-15s %-15s %-20s\n", "Loan ID", "User ID", "Loan Amount", "Assigned Emp.", "Loan Status");
    printf("-------------------------------------------------------------\n");

    // Read the file record by record
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel)) {
        // Display the loan details
        printf("%-10d %-10d %-15d %-15d %-20s\n",
               loan.loan_id,
               loan.user_id,
               loan.loan_amount,
               loan.employee_assigned,
               getLoanStatus(loan.loanStatus));
    }

    close(fd);
}

#endif