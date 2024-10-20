#include <stdio.h>
#include "../../models/customer_response_model.h"

CustomerResponseModel printCustomerMenu(){
    printf("\n---Customer Menu Controls---\n");
    printf("1. View Account Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Apply for a Loan\n");
    printf("5. Add a Feedback\n");
    printf("8. Logout\n");
    printf("9. Exit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    CustomerResponseModel customerResponseModel;
    if(choice == 1){
        customerResponseModel.operation = VIEW_BALANCE;
    } else if(choice == 8){
        customerResponseModel.operation = LOGOUT;
    } else if(choice == 9){
        customerResponseModel.operation = EXIT;
    } else {
        printf("Enter correct option!\n");
        customerResponseModel.operation = ERROR;
    }
    return customerResponseModel;
}