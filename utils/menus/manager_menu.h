// #include <stdio.h>
// #include "../../models/customer_response_model.h"

// CustomerResponseModel printManagerMenu()
// {
//     printf("\n---Customer Menu Controls---\n");
//     printf("1. Assign Loan to employee\n");
//     printf("8. Logout\n");
//     printf("9. Exit\n");

//     int choice;
//     printf("Enter your choice: ");
//     scanf("%d", &choice);
//     CustomerResponseModel customerResponseModel;
//     if (choice == 1)
//     {
//         customerResponseModel.operation = VIEW_BALANCE;
//     }
//     else if (choice == 2)
//     {
//         customerResponseModel.operation = DEPOSIT_MONEY;
//         char str[10];
//         printf("Enter amount to deposit: ");
//         scanf("%s", str);
//         strcpy(customerResponseModel.customerResponse, str);
//     }
//     else if (choice == 3)
//     {
//         customerResponseModel.operation = WITHDRAW_MONEY;
//         char str[10];
//         printf("Enter amount to withdraw: ");
//         scanf("%s", str);
//         strcpy(customerResponseModel.customerResponse, str);
//     }
//     else if (choice == 4)
//     {
//         customerResponseModel.operation = TRANSACT_MONEY;
//         char str[100], str2[10], combined[111];

//         printf("Enter username you want to transfer: ");
//         scanf("%s", str);
//         printf("Enter amount to transfer: ");
//         scanf("%s", str2);
//         snprintf(combined, sizeof(combined), "%s %s", str, str2);
//         strcpy(customerResponseModel.customerResponse, combined);
//     }
//     else if (choice == 5)
//     {
//         customerResponseModel.operation = VIEW_TRANSACTION_HISTORY;
//     }
//     else if (choice == 6)
//     {
//         customerResponseModel.operation = APPLY_LOAN;
//         char str[10];
//         printf("Enter amount of loan you want to apply: ");
//         scanf("%s", str);
//         strcpy(customerResponseModel.customerResponse, str);
//     }

//     else if (choice == 8)
//     {
//         customerResponseModel.operation = LOGOUT;
//     }
//     else if (choice == 9)
//     {
//         customerResponseModel.operation = EXIT;
//     }
//     else
//     {
//         printf("Enter correct option!\n");
//         customerResponseModel.operation = ERROR;
//     }
//     return customerResponseModel;
// }