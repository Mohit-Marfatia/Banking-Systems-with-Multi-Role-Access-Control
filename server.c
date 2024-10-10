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

    clientSD = accept(serverSD, (struct sockaddr *)(&client), &sz);

    int choice;
    const char *enterUname = enterUsername();
    const char *enterPw = enterPassword();

    UserModel userModel;
    bool flag = true;
    const char *displayUL = displayUserLogin();
    const char *displayAM = displayAdminMenu();
    do
    {
        // Send the login menu to the client
        write(clientSD, displayUL, strlen(displayUL) + 1);

        // Read the user's input (UserModel)
        read(clientSD, &userModel, sizeof(userModel));

        // Handle user exit
        if (userModel.user_id == -2)
        {
            if (userModel.isLoggedIn)
            {
                ResponseModel responseModel = getUserId(filename, userModel);
                // char serverMsg[100];
                int userId = atoi(responseModel.serverMessage);
                userModel.isLoggedIn = false;
                updateUser(filename, userId, userModel);
            }
            flag = false;
            break;
        }
        else
        {
            // Print user details
            printf("\nRole: %s\n", getRoleName(userModel.role));
            printf("Username: %s\n", userModel.username);
            printf("Password: %s\n", userModel.password);
            printf("UserId: %d\n", userModel.user_id);

            ResponseModel responseModel = getUserId(filename, userModel);
            // char serverMsg[100];
            int userId = atoi(responseModel.serverMessage);

            userModel.user_id = userId;
            userModel.accStatus = ENABLED;
            userModel.isLoggedIn = true;
            updateUser(filename, userId, userModel);
            strcpy(responseModel.serverMessage, "");
            readUsers(filename);
            write(clientSD, &responseModel, sizeof(responseModel));

            if (responseModel.statusCode == 400)
            {
                continue;
            }
            else
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
                        ResponseModel responseModel = getUserId(filename, userModel);
                        // char serverMsg[100];
                        int userId = atoi(responseModel.serverMessage);
                        userModel.isLoggedIn = false;
                        updateUser(filename, userId, userModel);
                        printf("\n Logout Successful!\n");
                        continue;
                    }
                    else if (userAuthModel.opereation == EXIT)
                    {
                        ResponseModel responseModel = getUserId(filename, userModel);
                        // char serverMsg[100];
                        int userId = atoi(responseModel.serverMessage);
                        userModel.isLoggedIn = false;
                        updateUser(filename, userId, userModel);
                        flag = false;
                        break;
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
