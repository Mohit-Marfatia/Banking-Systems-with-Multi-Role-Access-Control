#include <stdio.h>
#include "../../models/customer_response_model.h"
#include "../../models/user_model.h"
#include "../../enums/loan_status.h"

CustomerResponseModel printEmployeeMenu()
{
    printf("\n---Employee Menu Controls---\n");

    printf("1. Add a Customer\n");
    printf("2. Modify Customer Details\n");
    printf("3. Approve/Reject Loan\n");
    printf("4. Print all transactions of a customer:\n");
    printf("5. Change Password\n");
    printf("8. Logout\n");
    printf("9. Exit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    CustomerResponseModel customerResponseModel;
    if (choice == 1)
    {
        UserModel newUser;

        newUser.role = CUSTOMER;
        char username[100], password[100];
        printf("Enter username: ");
        scanf("%s", username);
        hidePasswordInput(password, sizeof(password));

        newUser.user_id = -1;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        
        char str[1024];
        userModelToString(newUser, str);
        strcpy(customerResponseModel.customerResponse, str);
        customerResponseModel.operation = ADD_CUSTOMER;
    }
    else if (choice == 2)
    {
        customerResponseModel.operation = MODIFY_CUSTOMER;
        // customerResponseModel.
    }
    else if (choice == 3)
    {
        customerResponseModel.operation = APPROVE_REJECT_LOAN;
    }
    else if (choice == 4)
    {
        customerResponseModel.operation = VIEW_TRANSACTION_HISTORY;
        readAllCustomers();
        printf("Enter user id of customer you want to see the transactions of: ");
        int userId;
        scanf("%d", &userId);
        sprintf(customerResponseModel.customerResponse, "%d", userId);
    }
    else if (choice == 5)
    {
        customerResponseModel.operation = CHANGE_PASSWORD;
        char password[100];
        hidePasswordInput(password, sizeof(password));
        strcpy(customerResponseModel.customerResponse, password);
    }
    else if (choice == 8)
    {
        customerResponseModel.operation = LOGOUT;
    }
    else if (choice == 9)
    {
        customerResponseModel.operation = EXIT;
    }
    else
    {
        printf("Enter correct option!\n");
        customerResponseModel.operation = ERROR;
    }
    return customerResponseModel;
}

int getLoanIdForApproval()
{
    printf("Choose one of the pending loans: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

LoanStatus loanApproval()
{
    printf("Press 1 to APPROVE and 2 to REJECT:");
    int choice;
    scanf("%d", &choice);
    return choice == 1 ? APPROVED : REJECTED;
}

UserModel modifyCustomerDetails()
{
    UserModel userModel;
    int userId;
    readAllCustomers();
    printf("\nEnter the id of the user you want to modify details of:");
    scanf("%d", &userId);
    userModel = getUserModelFromId(userId);
    printf("\n---Employee Menu Controls (Modify Customer)---\n");
    printf("1. Change Password\n");
    printf("2. Activate/Deactivate\n");
    int options;

    printf("Enter your choice: ");
    scanf("%d", &options);

    switch (options)
    {
    case 1:
        char password[100];
        printf("Change password to:\n");
        hidePasswordInput(password, sizeof(password));
        strcpy(userModel.password, password);
        break;
    case 2:
        printf("Press 1 to ACTIVATE and 2 to DEACTIVATE\n");
        scanf("%d", &options);
        userModel.accStatus = options == 1 ? ACTIVATED : DEACTIVATED;
    default:
        break;
    }

    return userModel;
}