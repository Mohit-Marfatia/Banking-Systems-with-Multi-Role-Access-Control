#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include "definitions.h"
#include "utils/constants.h"
#include "utils/menus/user_login_menu.h"
#include "utils/menus/admin_menu.h"
#include "models/response_model.h"
#include "models/user_auth_model.h"

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
    const char *displayUL = displayUserLogin();
    const char *displayAM = displayAdminMenu();
    do
    {
        // Clear the buffer before reusing it
        memset(buffer, 0, sizeof(buffer));
        read(sd, buffer, sizeof(buffer));
        // printf("%s", buffer);
        if (strcmp(buffer, displayUL) == 0)
        {
            userAuthModel = userLoginMenu();
            user = userAuthModel.user;

            write(sd, &userAuthModel, sizeof(userAuthModel));
            if (userAuthModel.operation == EXIT)
            {
                flag = false;
                break;
            }

            ResponseModel response;
            read(sd, &response, sizeof(response));

            printf("%s\n", response.responseMessage);
        }
        else if (strcmp(buffer, displayAM) == 0)
        {
            UserAuthModel userAuthModel = printAdminMenu(user);
            // printf("---------here @ debug ---------------\n");
            // printf("%s\n", getOperationName(userAuthModel.opereation));
            // printUserModel(userAuthModel.user);

            write(sd, &userAuthModel, sizeof(userAuthModel));
            if (userAuthModel.operation == LOGOUT)
            {
                continue;
            }
            if (userAuthModel.operation == EXIT)
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
                ResponseModel modifyUserResponseModel;
                read(sd, &modifyUserResponseModel, sizeof(modifyUserResponseModel));
                if(modifyUserResponseModel.statusCode == 400){
                    printf("%s", modifyUserResponseModel.responseMessage);
                    continue;
                }
                else{
                    UserModel originalModel;
                    userModelFromString(modifyUserResponseModel.serverMessage, &originalModel);
                    UserModel modifiedUser = modifyUserDetails(originalModel);
                    write(sd, &modifiedUser, sizeof(modifiedUser));

                    read(sd, &modifyUserResponseModel, sizeof(modifyUserResponseModel));

                    printf("%s", modifyUserResponseModel.responseMessage);
                }
                
            }
        }
    } while (flag);

    close(sd);
    printf("Exited successfully.\n");
    return 0;
}
