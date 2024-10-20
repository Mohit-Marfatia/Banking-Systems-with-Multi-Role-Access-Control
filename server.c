#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#include "utils/utils.h"
#include "utils/constants.h"
#include "models/user_auth_model.h"
#include "models/response_model.h"
#include "models/customer_response_model.h"
#include "models/feedback_model.h"
#include "helper/auth_controller.h"
#include "helper/transaction_controller.h"
#include "helper/loan_controller.h"
#include "helper/feedback_controller.h"

int main()
{
    struct sockaddr_in server, client;
    int serverSD, sz, clientSD;
    char buffer[100];
    const char *filename = "db/user/user_database.bin";

    serverSD = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORTNO);

    bind(serverSD, (struct sockaddr *)(&server), sizeof(server));
    listen(serverSD, 5);
    sz = sizeof(client);

    int choice, childPid;
    bool flag = true, userLoggedIn = false;
    UserModel userModel;
    UserAuthModel userAuthModel;

    char str[100] = "Connection established";
    while (1)
    {
        clientSD = accept(serverSD, (struct sockaddr *)(&client), &sz);

        if ((childPid = fork()) == 0)
        {
            printf("Connection established with a client\n");
            write(clientSD, str, sizeof(str));

            while (flag)
            {
                // Send the login menu to the client
                write(clientSD, displayUserLogin, strlen(displayUserLogin) + 1);

                // Read the user's input for login
                read(clientSD, &userAuthModel, sizeof(userAuthModel));
                userModel = userAuthModel.user;

                if (userAuthModel.operation == ERROR)
                {
                    continue;
                }
                else if (userAuthModel.operation == EXIT)
                {
                    flag = false;
                    printf("\nClient Exited successfully.\n");
                    close(clientSD);
                    continue;
                }
                else
                {
                    // Print user details
                    printf("\nRole: %s\n", getRoleName(userModel.role));
                    printf("Username: %s\n", userModel.username);
                    printf("Password: %s\n", userModel.password);

                    ResponseModel responseModel = getUserId(userAuthModel);
                    // char serverMsg[100];
                    printf("-----%d----\n", responseModel.statusCode);
                    // printf("-----------------%d------\n", userId);
                    if (responseModel.statusCode == 400)
                    {
                        write(clientSD, &responseModel, sizeof(responseModel));
                        continue;
                    }
                    else
                    {
                        int userId = atoi(responseModel.serverMessage);
                        userModel.user_id = userId;
                        //  printf("id---%d\n", userId);
                        ResponseModel loginResponse = login(userId, userModel);
                        if (loginResponse.statusCode == 400)
                        {
                            write(clientSD, &loginResponse, sizeof(loginResponse));
                            continue;
                        }
                        else
                        {
                            printf("UserId: %d\n", userModel.user_id);
                            userModel.accStatus = ACTIVATED;
                            userModel.isLoggedIn = true;
                            updateUser(userId, userModel);
                            strcpy(loginResponse.serverMessage, "");
                            // readUsers(filename);
                            write(clientSD, &loginResponse, sizeof(loginResponse));
                            userLoggedIn = true;

                            while (userLoggedIn)
                            {
                                if (userModel.role == SUPERADMIN || userModel.role == ADMIN)
                                {
                                    write(clientSD, displayAdminMenu, strlen(displayAdminMenu) + 1);

                                    UserAuthModel userAuthModel;
                                    read(clientSD, &userAuthModel, sizeof(userAuthModel));
                                    printf("\nOpr: %s\n", getOperationName(userAuthModel.operation));
                                    // printUserModel(userAuthModel.user);

                                    if (userAuthModel.operation == ERROR)
                                    {
                                        continue;
                                    }
                                    else if (userAuthModel.operation == LOGOUT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        printf("\n Logout Successful!\n");
                                        userLoggedIn = false;
                                        continue;
                                    }
                                    else if (userAuthModel.operation == EXIT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        userLoggedIn = false;
                                        flag = false;
                                        printf("Client Exited successfully.\n");
                                        close(clientSD);
                                        continue;
                                    }
                                    else if (userAuthModel.operation == ADD_ADMIN || userAuthModel.operation == ADD_MANAGER || userAuthModel.operation == ADD_EMPLOYEE || userAuthModel.operation == ADD_CUSTOMER)
                                    {
                                        createUser(userAuthModel.user);
                                        ResponseModel addUserResponseModel;
                                        strcpy(addUserResponseModel.responseMessage, "User Created Successfully");
                                        addUserResponseModel.statusCode = 200;
                                        write(clientSD, &addUserResponseModel, sizeof(addUserResponseModel));
                                    }
                                    else if (userAuthModel.operation == MODIFY_ADMIN || userAuthModel.operation == MODIFY_MANAGER || userAuthModel.operation == MODIFY_EMPLOYEE || userAuthModel.operation == MODIFY_CUSTOMER)
                                    {
                                        switch (userAuthModel.operation)
                                        {
                                        case MODIFY_ADMIN:
                                            write(clientSD, printAdminUsers, strlen(printAdminUsers) + 1);
                                            break;
                                        case MODIFY_MANAGER:
                                            write(clientSD, printManagerUsers, strlen(printManagerUsers) + 1);
                                            break;
                                        case MODIFY_EMPLOYEE:
                                            write(clientSD, printEmployeeUsers, strlen(printEmployeeUsers) + 1);
                                            break;
                                        case MODIFY_CUSTOMER:
                                            write(clientSD, printCustomerUsers, strlen(printCustomerUsers) + 1);
                                            break;

                                        default:
                                            break;
                                        }

                                        UserModel modifiedModel;
                                        read(clientSD, &modifiedModel, sizeof(UserModel));
                                        ResponseModel updateResponse = updateUser(modifiedModel.user_id, modifiedModel);
                                        write(clientSD, &updateResponse, sizeof(ResponseModel));
                                    }
                                }
                                else if (userModel.role == CUSTOMER)
                                {
                                    write(clientSD, displayCustomerMenu, strlen(displayCustomerMenu) + 1);
                                    CustomerResponseModel customerResponseModel;
                                    read(clientSD, &customerResponseModel, sizeof(customerResponseModel));
                                    printf("\nOpr: %s\n", getOperationName(customerResponseModel.operation));
                                    if (customerResponseModel.operation == ERROR)
                                    {
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == LOGOUT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        printf("\n Logout Successful!\n");
                                        userLoggedIn = false;
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == EXIT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        userLoggedIn = false;
                                        flag = false;
                                        printf("Client Exited successfully.\n");
                                        close(clientSD);
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == VIEW_BALANCE)
                                    {
                                        char *str = readAccountsOfUserId(userModel.user_id);
                                        int strSize = strlen(str) + 1;
                                        write(clientSD, &strSize, sizeof(strSize));
                                        write(clientSD, str, strlen(str) + 1);
                                    }
                                    else if (customerResponseModel.operation == DEPOSIT_MONEY)
                                    {
                                        char str[10];
                                        strcpy(str, customerResponseModel.customerResponse);
                                        int amount = atoi(str);
                                        transactMoney(userModel.user_id, userModel.user_id, amount, SAVINGS, DEPOSIT);
                                    }
                                    else if (customerResponseModel.operation == WITHDRAW_MONEY)
                                    {
                                        char str[10];
                                        strcpy(str, customerResponseModel.customerResponse);
                                        int amount = atoi(str);
                                        transactMoney(userModel.user_id, userModel.user_id, amount, SAVINGS, WITHDRAWAL);
                                    }
                                    else if (customerResponseModel.operation == TRANSACT_MONEY)
                                    {
                                        char str[100], str2[10];
                                        sscanf(customerResponseModel.customerResponse, "%s %s", str, str2);
                                        int toCustomer = getCustomerId(str);
                                        int amount = atoi(str2);
                                        transactMoney(userModel.user_id, toCustomer, amount, SAVINGS, TRANSFER);
                                    }
                                    else if (customerResponseModel.operation == VIEW_TRANSACTION_HISTORY)
                                    {
                                        char *str = readTransactionsOfUserId(userModel.user_id);
                                        int strSize = strlen(str) + 1;
                                        write(clientSD, &strSize, sizeof(strSize));
                                        write(clientSD, str, strlen(str) + 1);
                                    }
                                    else if (customerResponseModel.operation == APPLY_LOAN)
                                    {
                                        char str[100];
                                        strcpy(str, customerResponseModel.customerResponse);
                                        int amount = atoi(str);
                                        int apply = applyForLoan(userModel.user_id, amount);
                                        // if (apply == 0)
                                        //     strcpy(str, "Loan Applied successfully!\n");
                                        // else
                                        //     strcpy(str, "Something went wrong! Try again later\n");
                                        // write(clientSD, str, strlen(str) + 1);
                                        // continue;
                                    }
                                    else if (customerResponseModel.operation == ADD_FEEDBACK){
                                        addFeedback(userModel.user_id, customerResponseModel.customerResponse);
                                    }
                                }
                                else if (userModel.role == MANAGER)
                                {
                                    write(clientSD, displayManagerMenu, strlen(displayManagerMenu) + 1);
                                    CustomerResponseModel customerResponseModel;
                                    read(clientSD, &customerResponseModel, sizeof(customerResponseModel));
                                    printf("\nOpr: %s\n", getOperationName(customerResponseModel.operation));
                                    if (customerResponseModel.operation == ERROR)
                                    {
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == LOGOUT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        printf("\n Logout Successful!\n");
                                        userLoggedIn = false;
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == EXIT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        userLoggedIn = false;
                                        flag = false;
                                        printf("Client Exited successfully.\n");
                                        close(clientSD);
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == ASSIGN_LOAN_TO_EMPLOYEE)
                                    {
                                        char *str = printAllLoans();
                                        int strSize = strlen(str) + 1;
                                        write(clientSD, &strSize, sizeof(strSize));
                                        write(clientSD, str, strlen(str) + 1);
                                        int loanId;
                                        read(clientSD, &loanId, sizeof(loanId));

                                        str = printAllEmployees();
                                        strSize = strlen(str) + 1;
                                        write(clientSD, &strSize, sizeof(strSize));
                                        write(clientSD, str, strlen(str) + 1);
                                        int empId;
                                        read(clientSD, &empId, sizeof(empId));

                                        int assign = assignLoanToEmployee(loanId, empId);
                                        // if (assign == 0)

                                        //     strcpy(str, "Loan Assigned successfully!\n");
                                        // else
                                        //     strcpy(str, "Something went wrong! Try again later\n");
                                        // write(clientSD, str, strlen(str) + 1);
                                        // continue;
                                        // continue;
                                    } else if(customerResponseModel.operation == VIEW_FEEDBACK){
                                         char *str = printAllFeedbacks();
                                        int strSize = strlen(str) + 1;
                                        write(clientSD, &strSize, sizeof(strSize));
                                        write(clientSD, str, strlen(str) + 1);
                                    }
                                }
                                else if (userModel.role == EMPLOYEE)
                                {
                                    write(clientSD, displayEmployeeMenu, strlen(displayEmployeeMenu) + 1);
                                    CustomerResponseModel customerResponseModel;
                                    read(clientSD, &customerResponseModel, sizeof(customerResponseModel));
                                    printf("\nOpr: %s\n", getOperationName(customerResponseModel.operation));
                                    if (customerResponseModel.operation == ERROR)
                                    {
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == LOGOUT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        printf("\n Logout Successful!\n");
                                        userLoggedIn = false;
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == EXIT)
                                    {
                                        logout(userModel.user_id, userModel);
                                        userLoggedIn = false;
                                        flag = false;
                                        printf("Client Exited successfully.\n");
                                        close(clientSD);
                                        continue;
                                    }
                                    else if (customerResponseModel.operation == APPROVE_REJECT_LOAN)
                                    {
                                        char *str = printAllLoansForEmployee(userModel.user_id);
                                        int strSize = strlen(str) + 1;
                                        write(clientSD, &strSize, sizeof(strSize));
                                        write(clientSD, str, strlen(str) + 1);
                                        int loanId;
                                        read(clientSD, &loanId, sizeof(loanId));
                                        LoanStatus approve;
                                        read(clientSD, &approve, sizeof(approve));
                                        int approveResponse = approveOrRejectLoan(loanId, approve);
                                        if (approve == APPROVED)
                                        {
                                            int loan_amount = addLoanAccount(loanId);
                                            int user_id = getUserIdFromLoanId(loanId);
                                            transactMoney(user_id, user_id, loan_amount, SAVINGS, DEPOSIT);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    close(serverSD);
    return 0;
}