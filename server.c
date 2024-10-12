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
// #include "models/user_model.h"
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

    clientSD = accept(serverSD, (struct sockaddr *)(&client), &sz);

    int choice;
    const char *enterUname = enterUsername();
    const char *enterPw = enterPassword();

    UserModel userModel;
    UserAuthModel userAuthModel;
    bool flag = true;
    bool userLoggedIn = false;
    const char *displayUL = displayUserLogin();
    const char *displayAM = displayAdminMenu();
    do
    {
        // Send the login menu to the client
        write(clientSD, displayUL, strlen(displayUL) + 1);

        // Read the user's input (UserModel)
        read(clientSD, &userAuthModel, sizeof(userAuthModel));
        userModel = userAuthModel.user;
        // Handle user exit 
        if (userAuthModel.opereation == EXIT)
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
            break;
        }
        else
        {
            // Print user details
            printf("\nRole: %s\n", getRoleName(userModel.role));
            printf("Username: %s\n", userModel.username);
            printf("Password: %s\n", userModel.password);

            ResponseModel responseModel = getUserId(filename, userModel);
            // char serverMsg[100];
            int userId = atoi(responseModel.serverMessage);
            printf("-----------------%d------\n", userId);
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
                        printf("\nOpr: %s\n", getOperationName(userAuthModel.opereation));
                        // printUserModel(userAuthModel.user);

                        if (userAuthModel.opereation == LOGOUT)
                        {
                            ResponseModel getIdLResponseModel = getUserId(filename, userModel);
                            // char serverMsg[100];
                            int userId = atoi(getIdLResponseModel.serverMessage);
                            userModel.isLoggedIn = false;
                            updateUser(filename, userId, userModel);
                            printf("\n Logout Successful!\n");
                            userLoggedIn = false;
                            break;
                        }
                        else if (userAuthModel.opereation == EXIT)
                        {
                            ResponseModel getIdEesponseModel = getUserId(filename, userModel);
                            // char serverMsg[100];
                            int userId = atoi(getIdEesponseModel.serverMessage);
                            userModel.isLoggedIn = false;
                            updateUser(filename, userId, userModel);
                            userLoggedIn = false;
                            flag = false;
                            break;
                        }
                        else if (userAuthModel.opereation == ADD_ADMIN || userAuthModel.opereation == ADD_MANAGER || userAuthModel.opereation == ADD_EMPLOYEE || userAuthModel.opereation == ADD_CUSTOMER)
                        {
                            ResponseModel addUserResponseModel = createNewUser(filename, userAuthModel);
                            printf("%s", addUserResponseModel.responseMessage);
                            write(clientSD, &addUserResponseModel, sizeof(addUserResponseModel));
                        }
                    }
                }
            }
        }
    } while (flag);

    printf("Exited successfully.\n");
    close(clientSD);
    close(serverSD);

    return 0;
}
