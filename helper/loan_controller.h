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

    printf("---@loan-debug---\n loan id: %d", id);
    return id;
}

int applyForLoan(int user_id, int loan_amount)
{
    LoanModel loan;
    int fd = open(loanDatabase, O_WRONLY | O_APPEND | O_CREAT, 0600);
    if (fd == -1)
    {
        perror("Error opening loan database");
        return -1;
    }

    // Lock the loan database for writing
    if (lockTransactionDb(fd, F_WRLCK) == -1)
    {
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
    if (write(fd, &loan, sizeof(LoanModel)) == -1)
    {
        perror("Error writing to loan database");
        lockTransactionDb(fd, F_UNLCK);
        close(fd);
        return -1;
    }

    // Unlock and close the file
    lockTransactionDb(fd, F_UNLCK);
    close(fd);

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

    model.loanCount++;

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

    printf("Loan applied successfully.\n");
    return 0;
}

int assignLoanToEmployee(int loan_id, int employee_id)
{
    LoanModel loan;
    int fd = open(loanDatabase, O_RDWR, 0600);
    if (fd == -1)
    {
        perror("Error opening loan database");
        return -1;
    }

    if (lockTransactionDb(fd, F_WRLCK) == -1)
    {
        perror("Error locking loan database");
        close(fd);
        return -1;
    }

    // Find the loan in the loan database
    off_t pos = lseek(fd, 0, SEEK_SET);
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {
        if (loan.loan_id == loan_id)
        {
            loan.employee_assigned = employee_id;

            // Seek back to the position of this loan to update it
            lseek(fd, pos, SEEK_SET);
            if (write(fd, &loan, sizeof(LoanModel)) == -1)
            {
                perror("Error updating loan in database");
                lockTransactionDb(fd, F_UNLCK);
                close(fd);
                return -1;
            }
            break;
        }
        pos = lseek(fd, 0, SEEK_CUR); // Move position to next record
    }

    lockTransactionDb(fd, F_UNLCK);
    close(fd);

    printf("Loan assigned to employee successfully.\n");
    return 0;
}

int approveOrRejectLoan(int loan_id, LoanStatus status)
{
    LoanModel loan;
    int fd = open(loanDatabase, O_RDWR, 0600);
    if (fd == -1)
    {
        perror("Error opening loan database");
        return -1;
    }

    if (lockTransactionDb(fd, F_WRLCK) == -1)
    {
        perror("Error locking loan database");
        close(fd);
        return -1;
    }

    // Find the loan in the loan database
    off_t pos = lseek(fd, 0, SEEK_SET);
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {
        if (loan.loan_id == loan_id)
        {
            loan.loanStatus = status;

            // Seek back to the position of this loan to update it
            lseek(fd, pos, SEEK_SET);
            if (write(fd, &loan, sizeof(LoanModel)) == -1)
            {
                perror("Error updating loan in database");
                lockTransactionDb(fd, F_UNLCK);
                close(fd);
                return -1;
            }
            break;
        }
        pos = lseek(fd, 0, SEEK_CUR); // Move position to next record
    }

    lockTransactionDb(fd, F_UNLCK);
    close(fd);

    printf("Loan status updated to %s.\n", status == APPROVED ? "APPROVED" : "REJECTED");
    return 0;
}

int getUserIdFromLoanId(int loanId)
{
    LoanModel loan;
    int user_id;
    int fd = open(loanDatabase, O_RDWR, 0600);
    if (fd == -1)
    {
        perror("Error opening loan database");
        return -1;
    }

    if (lockTransactionDb(fd, F_WRLCK) == -1)
    {
        perror("Error locking loan database");
        close(fd);
        return -1;
    }

    // Find the loan in the loan database
    off_t pos = lseek(fd, 0, SEEK_SET);
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {
        if (loan.loan_id == loanId)
        {
            user_id = loan.user_id;
            break;
        }
        pos = lseek(fd, 0, SEEK_CUR); // Move position to next record
    }

    lockTransactionDb(fd, F_UNLCK);
    close(fd);
    return user_id;
}

int addLoanAccount(int loanId)
{
    LoanModel loan;
    AccountModel account;
    int user_id, loan_amount;
    int fd = open(loanDatabase, O_RDWR, 0600);
    if (fd == -1)
    {
        perror("Error opening loan database");
        return -1;
    }

    if (lockTransactionDb(fd, F_WRLCK) == -1)
    {
        perror("Error locking loan database");
        close(fd);
        return -1;
    }

    // Find the loan in the loan database
    off_t pos = lseek(fd, 0, SEEK_SET);
    int found_loan = 0;
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {
        if (loan.loan_id == loanId)
        {
            user_id = loan.user_id;
            loan_amount = loan.loan_amount;
            found_loan = 1;
            break;
        }
        pos = lseek(fd, 0, SEEK_CUR); // Move position to next record
    }

    lockTransactionDb(fd, F_UNLCK);
    close(fd);

    if (!found_loan)
    {
        printf("Loan with ID %d not found\n", loanId);
        return -1;
    }

    // Open the account database to search for an existing loan account
    fd = open(accountDatabase, O_RDWR, 0600); // Changed from O_APPEND to O_RDWR
    if (fd == -1)
    {
        perror("Error opening account database");
        return -1;
    }

    if (lockAccountDb(fd, F_WRLCK) == -1)
    {
        perror("Error locking account database");
        close(fd);
        return -1;
    }

    int found_account = 0;
    off_t account_pos = lseek(fd, 0, SEEK_SET);

    // Search for an existing loan account for the user
    while (read(fd, &account, sizeof(AccountModel)) == sizeof(AccountModel))
    {
        if (account.user_id == user_id && account.accType == LOAN)
        {
            // Loan account found, update balance
            account.balance -= loan_amount; // Adding the new loan amount
            found_account = 1;
            break;
        }
        account_pos = lseek(fd, 0, SEEK_CUR); // Move position to next record
    }

    if (found_account)
    {
        // Move back to the correct position to update the account in-place
        lseek(fd, account_pos - sizeof(AccountModel), SEEK_SET);

        // Update the account record with the new loan balance
        if (write(fd, &account, sizeof(AccountModel)) == -1)
        {
            perror("Error updating account in database");
            lockAccountDb(fd, F_UNLCK);
            close(fd);
            return -1;
        }

        printf("Existing loan account updated for user ID %d with new balance: %d\n", user_id, account.balance);
    }
    else
    {
        // If no existing loan account was found, create a new loan account
        account.account_id = getNewAccountId();
        account.user_id = user_id;
        account.accType = LOAN;
        account.balance = -1 * loan_amount;

        if (write(fd, &account, sizeof(AccountModel)) == -1)
        {
            perror("Error writing new account to database");
            lockAccountDb(fd, F_UNLCK);
            close(fd);
            return -1;
        }

        printf("New loan account created for user ID %d with loan amount: %d\n", user_id, loan_amount);
    }

    lockAccountDb(fd, F_UNLCK);
    close(fd);

    return loan_amount;
}

void readAllLoans()
{
    LoanModel loan;
    int fd = open(loanDatabase, O_RDONLY, 0600); // Assuming loanDatabase.db is the filename
    if (fd == -1)
    {
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
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {
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

char *printAllEmployees()
{
    UserIdModel user;
    UserModel userModel;
    int fd = open(employeeDatabase, O_RDONLY, 0600);
    if (fd == -1)
    {
        perror("Error opening employee database");
        return NULL;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    // Initial size for the output string buffer
    size_t buffer_size = 1024;
    size_t content_length = 0;

    // Dynamically allocate memory for the result string
    char *result = (char *)malloc(buffer_size * sizeof(char));
    if (result == NULL)
    {
        perror("Failed to allocate memory");
        close(fd);
        return NULL;
    }

    // Initialize the string buffer
    result[0] = '\0'; // Set the string to empty initially

    // Append header to the buffer
    content_length += snprintf(result + content_length, buffer_size - content_length,
                               "\n%-10s %-20s %-15s %-10s %-10s\n", "ID", "Username", "Role", "LoggedIn", "Status");

    // Append the separator
    content_length += snprintf(result + content_length, buffer_size - content_length,
                               "-------------------------------------------------------------------\n");

    // Read the file record by record
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {
        if (user.user_id != -1)
        { // Only display records that are not marked as deleted

            // Fetch the full user model details
            userModel = getUserModelFromId(user.user_id);

            // Ensure there's enough space to append the new user data
            if (content_length + 200 >= buffer_size)
            {
                buffer_size *= 2; // Double the buffer size
                result = (char *)realloc(result, buffer_size * sizeof(char));
                if (result == NULL)
                {
                    perror("Failed to reallocate memory");
                    close(fd);
                    return NULL;
                }
            }

            // Append user details to the buffer
            content_length += snprintf(result + content_length, buffer_size - content_length,
                                       "%-10d %-20s %-15s %-10s %-10s\n",
                                       userModel.user_id,
                                       userModel.username,
                                       getRoleName(userModel.role),
                                       userModel.isLoggedIn ? "true" : "false",
                                       getAccountStatus(userModel.accStatus));
        }
    }

    close(fd);
    return result; // Return the dynamically allocated string
}

char *printAllLoans()
{
    LoanModel loan;
    int fd = open(loanDatabase, O_RDONLY, 0600); // Assuming loanDatabase is the filename
    if (fd == -1)
    {
        perror("Error opening loan database");
        return NULL;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    // Initial size for the output string buffer
    size_t buffer_size = 1024;
    size_t content_length = 0;

    // Dynamically allocate memory for the result string
    char *result = (char *)malloc(buffer_size * sizeof(char));
    if (result == NULL)
    {
        perror("Failed to allocate memory");
        close(fd);
        return NULL;
    }

    // Initialize the string buffer
    result[0] = '\0'; // Set the string to empty initially

    // Append header to the buffer
    content_length += snprintf(result + content_length, buffer_size - content_length,
                               "\n%-10s %-10s %-15s %-15s %-20s\n", "Loan ID", "User ID", "Loan Amount", "Assigned Emp.", "Loan Status");

    // Append the separator
    content_length += snprintf(result + content_length, buffer_size - content_length,
                               "-------------------------------------------------------------\n");

    // Read the file record by record
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {

        // Ensure there's enough space to append the new loan data
        if (content_length + 200 >= buffer_size)
        {
            buffer_size *= 2; // Double the buffer size
            result = (char *)realloc(result, buffer_size * sizeof(char));
            if (result == NULL)
            {
                perror("Failed to reallocate memory");
                close(fd);
                return NULL;
            }
        }

        // Append loan details to the buffer
        content_length += snprintf(result + content_length, buffer_size - content_length,
                                   "%-10d %-10d %-15d %-15d %-20s\n",
                                   loan.loan_id,
                                   loan.user_id,
                                   loan.loan_amount,
                                   loan.employee_assigned,
                                   getLoanStatus(loan.loanStatus));
    }

    close(fd);
    return result; // Return the dynamically allocated string
}

char *printAllLoansForEmployee(int user_id)
{
    LoanModel loan;
    int fd = open(loanDatabase, O_RDONLY, 0600); // Assuming loanDatabase is the filename
    if (fd == -1)
    {
        perror("Error opening loan database");
        return NULL;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    // Initial size for the output string buffer
    size_t buffer_size = 1024;
    size_t content_length = 0;

    // Dynamically allocate memory for the result string
    char *result = (char *)malloc(buffer_size * sizeof(char));
    if (result == NULL)
    {
        perror("Failed to allocate memory");
        close(fd);
        return NULL;
    }

    // Initialize the string buffer
    result[0] = '\0'; // Set the string to empty initially

    // Append header to the buffer
    content_length += snprintf(result + content_length, buffer_size - content_length,
                               "\n%-10s %-10s %-15s %-15s %-20s\n", "Loan ID", "User ID", "Loan Amount", "Assigned Emp.", "Loan Status");

    // Append the separator
    content_length += snprintf(result + content_length, buffer_size - content_length,
                               "-------------------------------------------------------------\n");

    // Read the file record by record
    while (read(fd, &loan, sizeof(LoanModel)) == sizeof(LoanModel))
    {
        // Only append loans assigned to the given employee (user_id)
        if (loan.employee_assigned == user_id)
        {
            // Ensure there's enough space to append the new loan data
            if (content_length + 200 >= buffer_size)
            {
                buffer_size *= 2; // Double the buffer size
                result = (char *)realloc(result, buffer_size * sizeof(char));
                if (result == NULL)
                {
                    perror("Failed to reallocate memory");
                    close(fd);
                    return NULL;
                }
            }

            // Append loan details to the buffer
            content_length += snprintf(result + content_length, buffer_size - content_length,
                                       "%-10d %-10d %-15d %-15d %-20s\n",
                                       loan.loan_id,
                                       loan.user_id,
                                       loan.loan_amount,
                                       loan.employee_assigned,
                                       getLoanStatus(loan.loanStatus));
        }
    }

    close(fd);
    return result; // Return the dynamically allocated string
}

#endif