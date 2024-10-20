#include <stdio.h>
#include "../../models/customer_response_model.h"
#include "../../enums/loan_status.h"

CustomerResponseModel printEmployeeMenu()
{
    printf("\n---Customer Menu Controls---\n");
    printf("1. Approve/Reject Loan\n");
    printf("8. Logout\n");
    printf("9. Exit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    CustomerResponseModel customerResponseModel;
    if (choice == 1)
    {
        customerResponseModel.operation = APPROVE_REJECT_LOAN;
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

int getLoanIdForApproval(){
    printf("Choose one of the pending loans: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

LoanStatus loanApproval(){
    printf("Press 1 to APPROVE and 2 to REJECT:");
    int choice;
    scanf("%d", &choice);
    return choice == 1? APPROVED : REJECTED;
}