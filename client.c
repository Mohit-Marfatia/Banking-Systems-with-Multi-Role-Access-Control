#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include "utils/utils.h"
#include "utils/constants.h"
#include "utils/menus/user_login_menu.h"
#include "utils/menus/admin_menu.h"
#include "utils/menus/customer_menu.h"
#include "models/user_auth_model.h"
#include "models/response_model.h"
#include "models/customer_response_model.h"
#include "helper/auth_controller.h"

int main()
{
    struct sockaddr_in server;
    int sd;
    char buffer[100];

    sd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORTNO);

    connect(sd, (struct sockaddr *)(&server), sizeof(server));

    bool flag = true;
    UserModel user;
    UserAuthModel userAuthModel;

    while (flag)
    {
        // Clear the buffer before reusing it
        memset(buffer, 0, sizeof(buffer));
        read(sd, buffer, sizeof(buffer));

        // printf("%s | %s\n", buffer, displayUserLogin);
        if (strcmp(buffer, displayUserLogin) == 0)
        {
            userAuthModel = userLoginDetails();
            // printf("--debug--\n");
            // printf("%s\n", userAuthModel.user.isLoggedIn ? "true":"false");
            user = userAuthModel.user;

            write(sd, &userAuthModel, sizeof(userAuthModel));

            if (userAuthModel.operation == ERROR)
            {
                printf("\nPlease enter a correct option!\n");
                continue;
            }
            else if (userAuthModel.operation == EXIT)
            {
                flag = false;
                continue;
            }

            ResponseModel response;
            read(sd, &response, sizeof(response));

            printf("%s\n", response.responseMessage);
        }
        else if (strcmp(buffer, displayAdminMenu) == 0)
        {
            UserAuthModel userAuthModel = printAdminMenu(user);
            // printf("---------here @ debug ---------------\n");
            // printf("%s\n", getOperationName(userAuthModel.opereation));
            // printUserModel(userAuthModel.user);

            write(sd, &userAuthModel, sizeof(userAuthModel));
            if (userAuthModel.operation == ERROR)
            {
                printf("\nPlease enter a correct option!\n");
                continue;
            }
            else if (userAuthModel.operation == LOGOUT)
            {
                continue;
            }
            else if (userAuthModel.operation == EXIT)
            {
                flag = false;
                break;
            }
            else if (userAuthModel.operation == ADD_ADMIN || userAuthModel.operation == ADD_MANAGER || userAuthModel.operation == ADD_EMPLOYEE || userAuthModel.operation == ADD_CUSTOMER)
            {
                ResponseModel addUserResponseModel;
                read(sd, &addUserResponseModel, sizeof(addUserResponseModel));
                printf("%s", addUserResponseModel.responseMessage);
            }
            else if (userAuthModel.operation == MODIFY_ADMIN || userAuthModel.operation == MODIFY_MANAGER || userAuthModel.operation == MODIFY_EMPLOYEE || userAuthModel.operation == MODIFY_CUSTOMER)
            {
                memset(buffer, 0, sizeof(buffer));
                read(sd, buffer, sizeof(buffer));
                // printf("%s | %s\n", buffer, printManagerUsers);
                if (strcmp(buffer, printAdminUsers) == 0)
                {
                    readAllAdmin();
                }
                else if (strcmp(buffer, printManagerUsers) == 0)
                {
                    readAllManagers();
                }
                else if (strcmp(buffer, printEmployeeUsers) == 0)
                {
                    readAllEmployees();
                }
                else if (strcmp(buffer, printCustomerUsers) == 0)
                {
                    readAllCustomers();
                }

                UserModel modifiedModel = modifyUserDetails();
                write(sd, &modifiedModel, sizeof(UserModel));
                ResponseModel updateResponse;
                read(sd, &updateResponse, sizeof(ResponseModel));

                printf("%s\n", updateResponse.responseMessage);
            }
        }
        else if (strcmp(buffer, displayCustomerMenu) == 0)
        {
            CustomerResponseModel customerResponseModel = printCustomerMenu();
            write(sd, &customerResponseModel, sizeof(customerResponseModel));
            if (customerResponseModel.operation == ERROR)
            {
                continue;
            }
            else if (customerResponseModel.operation == LOGOUT)
            {
                continue;
            }
            else if (customerResponseModel.operation == EXIT)
            {
                flag = false;
                continue;
            }
            else if (customerResponseModel.operation == VIEW_BALANCE || customerResponseModel.operation == VIEW_TRANSACTION_HISTORY)
            {
                int strSize;
                read(sd, &strSize, sizeof(strSize));
                char str[strSize];
                read(sd, str, sizeof(str));
                printf("%s", str);
            } else if(customerResponseModel.operation == APPLY_LOAN){
                char str[100];
                read(sd, str, sizeof(str));
                printf("%s", str);
                continue;
            }
        }
    }

    close(sd);
    printf("Exited successfully.\n");
    return 0;
}
