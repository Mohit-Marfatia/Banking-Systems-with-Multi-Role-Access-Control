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
#include "helper/auth_controller.h"

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
                        ResponseModel loginResponse = login(userId, userModel);
                        if (loginResponse.statusCode == 400)
                        {
                            write(clientSD, &loginResponse, sizeof(loginResponse));
                            continue;
                        }
                        else
                        {
                            printf("UserId: %d\n", userModel.user_id);
                            userModel.accStatus = ENABLED;
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