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

int main() {
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
    const char* displayUL = displayUserLogin();
    const char* displayAM = displayAdminMenu();  
    do {
        // Clear the buffer before reusing it
        memset(buffer, 0, sizeof(buffer));
        read(sd, buffer, sizeof(buffer));
        // printf("%s", buffer);
        if(strcmp(buffer, displayUL) == 0){
            user = userLoginMenu();
            
            write(sd, &user, sizeof(user));
            if(user.user_id == -2){
                flag = false;
                break;
            }

            ResponseModel response;
            read(sd, &response, sizeof(response));

            printf("%s\n", response.responseMessage);
        } else if (strcmp(buffer, displayAM) == 0) {
            UserAuthModel userAuthModel = printAdminMenu(user);
            write(sd, &userAuthModel, sizeof(userAuthModel));
        }
    } while(flag);

    close(sd);

    return 0;
}


        