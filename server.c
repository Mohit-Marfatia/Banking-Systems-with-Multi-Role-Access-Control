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
#include "definitions.h"
#include "utils/constants.h"
#include "models/user_model.h"
#include "models/user_auth_model.h"
#include "helper/auth_controller.h"

int main()
{
    struct sockaddr_in server, client;
    int serverSD, sz, clientSD;
    char buffer[100];
    const char *filename = "db/user_database.bin";

    serverSD = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORTNO);

    bind(serverSD, (struct sockaddr *)(&server), sizeof(server));
    listen(serverSD, 5);
    sz = sizeof(client);

    int choice, childPid;
    const char *enterUname = enterUsername();
    const char *enterPw = enterPassword();

    UserModel userModel;
    UserAuthModel userAuthModel;
    bool flag = true;
    bool userLoggedIn = false;
    const char *displayUL = displayUserLogin();
    const char *displayAM = displayAdminMenu();
    while (1)
    {
        clientSD = accept(serverSD, (struct sockaddr *)(&client), &sz);

        if ((childPid = fork()) == 0)
        {

            do
            {
                // Send the login menu to the client
                write(clientSD, displayUL, strlen(displayUL) + 1);

                // Read the user's input (UserModel)
                read(clientSD, &userAuthModel, sizeof(userAuthModel));
                userModel = userAuthModel.user;
                // Handle user exit
                if (userAuthModel.operation == EXIT)
                {
                    // if (userModel.isLoggedIn)
                    // {
                    //     ResponseModel responseModel = getUserId(filename, userModel);
                    //     // char serverMsg[100];
                    //     int userId = atoi(responseModel.serverMessage);
                    //     userModel.user_id = userId;
                    //     userModel.isLoggedIn = false;
                    //     updateUser(filename, userId, userModel);
                    // }
                    flag = false;
                    printf("Client Exited successfully.\n");
                    close(clientSD);
                    continue;
                }
                else
                {
                    // Print user details
                    printf("\nRole: %s\n", getRoleName(userModel.role));
                    printf("Username: %s\n", userModel.username);
                    printf("Password: %s\n", userModel.password);

                    ResponseModel responseModel = getUserId(filename, userAuthModel);
                    // char serverMsg[100];
                    // printf("-----%d----", responseModel.statusCode);
                    int userId = atoi(responseModel.serverMessage);
                    // printf("-----------------%d------\n", userId);
                    if (responseModel.statusCode == 400)
                    {
                        continue;
                    }
                    else
                    {
                        userModel.user_id = userId;
                        printf("UserId: %d\n", userModel.user_id);
                        userModel.accStatus = ENABLED;
                        userModel.isLoggedIn = true;
                        updateUser(filename, userId, userModel);
                        strcpy(responseModel.serverMessage, "");
                        // readUsers(filename);
                        write(clientSD, &responseModel, sizeof(responseModel));
                        userLoggedIn = true;
                        while (userLoggedIn)
                        {
                            if (userModel.role == SUPERADMIN || userModel.role == ADMIN)
                            {
                                write(clientSD, displayAM, strlen(displayAM) + 1);

                                UserAuthModel userAuthModel;
                                read(clientSD, &userAuthModel, sizeof(userAuthModel));
                                printf("\nOpr: %s\n", getOperationName(userAuthModel.operation));
                                // printUserModel(userAuthModel.user);

                                if (userAuthModel.operation == LOGOUT)
                                {
                                    ResponseModel getIdLResponseModel = getUserId(filename, userAuthModel);
                                    // char serverMsg[100];
                                    int userId = atoi(getIdLResponseModel.serverMessage);
                                    userModel.isLoggedIn = false;
                                    updateUser(filename, userId, userModel);
                                    printf("\n Logout Successful!\n");
                                    userLoggedIn = false;
                                    continue;
                                    ;
                                }
                                else if (userAuthModel.operation == EXIT)
                                {
                                    ResponseModel getIdResponseModel = getUserId(filename, userAuthModel);
                                    // char serverMsg[100];
                                    int userId = atoi(getIdResponseModel.serverMessage);
                                    userModel.isLoggedIn = false;
                                    updateUser(filename, userId, userModel);
                                    userLoggedIn = false;
                                    flag = false;
                                    printf("Client Exited successfully.\n");
                                    close(clientSD);
                                    continue;
                                }
                                else if (userAuthModel.operation == ADD_ADMIN || userAuthModel.operation == ADD_MANAGER || userAuthModel.operation == ADD_EMPLOYEE || userAuthModel.operation == ADD_CUSTOMER)
                                {
                                    ResponseModel addUserResponseModel = createNewUser(filename, userAuthModel);
                                    printf("%s", addUserResponseModel.responseMessage);
                                    write(clientSD, &addUserResponseModel, sizeof(addUserResponseModel));
                                }
                                else if (userAuthModel.operation == MODIFY_ADMIN || userAuthModel.operation == MODIFY_MANAGER || userAuthModel.operation == MODIFY_EMPLOYEE || userAuthModel.operation == MODIFY_CUSTOMER)
                                {
                                    ResponseModel getIdResponseModel = getUserId(filename, userAuthModel);
                                    // char serverMsg[100];
                                    // UserModel originalData;
                                    // userModelFromString(getIdResponseModel.serverMessage, &originalData);
                                    // char str[5]; // Ensure the array is large enough to hold the number
                                    // sprintf(str, "%d", originalData.user_id); // Convert int to string

                                    // send the original user data to client
                                    write(clientSD, &getIdResponseModel, sizeof(getIdResponseModel));
                                    if (getIdResponseModel.statusCode == 400)
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        UserModel modifiedData;
                                        read(clientSD, &modifiedData, sizeof(modifiedData));
                                        ResponseModel responseModel = updateUser(filename, modifiedData.user_id, modifiedData);

                                        write(clientSD, &responseModel, sizeof(responseModel));
                                    }
                                }
                            }
                        }
                    }
                }
            } while (flag);
        }
    }

    close(serverSD);

    return 0;
}
