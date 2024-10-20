#include <stdio.h>
#include "../../models/customer_response_model.h"

CustomerResponseModel printManagerMenu()
{
    printf("\n---Manager Menu Controls---\n");
    printf("1. Assign Loan to employee\n");
    printf("2. View Feedbacks\n");
    printf("8. Logout\n");
    printf("9. Exit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    CustomerResponseModel customerResponseModel;
    if (choice == 1)
    {
        customerResponseModel.operation = ASSIGN_LOAN_TO_EMPLOYEE;
    }
    if (choice == 2)
    {
        customerResponseModel.operation = VIEW_FEEDBACK;
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

int getEmployeeIdForLoanAssignment(){
    printf("Choose one of the employees to assign the loan: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

int getLoanIdForLoanAssignment(){
    printf("Choose one of the pending loans: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}