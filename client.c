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
#include "utils/menus.h"

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
    const char* displayUL = displayUserLogin();  
    do {
        // Clear the buffer before reusing it
        memset(buffer, 0, sizeof(buffer));
        read(sd, buffer, sizeof(buffer));
        // printf("%s", buffer);
        if(strcmp(buffer, displayUL) == 0){
            UserModel userLogin = userLoginMenu();
            
            write(sd, &userLogin, sizeof(userLogin));
            if(userLogin.user_id == -2){
                flag = false;
                break;
            }
        }
    } while(flag);

    close(sd);

    return 0;
}


        