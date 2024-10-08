#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include "definitions.h"
#include "utils/constants.h"
#include "models/user_model.h"


int main() {
    struct sockaddr_in server, client;   
    int serverSD, sz, clientSD;
    char buffer[100];
    
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
    const char* displayUL = displayUserLogin();  // Display the login menu
    do {
        // Send the login menu to the client
        write(clientSD, displayUL, strlen(displayUL)+1);

        // Read the user's input (UserModel)
        read(clientSD, &userModel, sizeof(userModel));

        // Handle user exit
        if(userModel.user_id == -2){
            flag = false;
            break;
        } else {
            // Print user details
            printf("\nRole: %s\n", getRoleName(userModel.role));
            printf("Username: %s\n", userModel.username);
            printf("Password: %s\n", userModel.password);

            
        }
    } while (flag);    

    printf("Exited successfully.\n");
    close(clientSD);
    close(serverSD);

    return 0;
}

