#include <stdio.h>
#include "../models/user_model.h"
#include "../definitions.h"

UserModel userLoginMenu(){
    int choice;
    UserModel newUser;
    printf("\nLogin as:\n");
    printf("Press 1 for Customer\n");
    printf("Press 2 for Bank Employee\n");
    printf("Press 9 to exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if(choice == 1){
        char username[100], password[100];
        printf("Enter username: ");
        scanf("%s", username);
        hidePasswordInput(password, sizeof(password));

        // UserModel userModel = ;
        newUser.user_id = -1;
        newUser.role = CUSTOMER;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        return newUser;

    } else if(choice == 9){
    
        newUser.user_id = -2;
        newUser.role = CUSTOMER;
        strcpy(newUser.username, "");
        strcpy(newUser.password, "");
        return newUser;
    } else {
        printf("\nLogin as:\n");
        printf("Press 1 to login as Super Admin\n");
        printf("Press 2 to login as Admin\n");
        printf("Press 3 to login as Manager\n");
        printf("Press 4 to login as Employee\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        char username[100], password[100];
        printf("Enter username: ");
        scanf("%s", username);
        hidePasswordInput(password, sizeof(password));
        // UserModel userModel;

        newUser.user_id = -1;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);

        switch (choice) {
            case 1:
                // UserModel userModel = ;
                newUser.role = SUPERADMIN;
                return newUser;
            case 2:
                // UserModel userModel = ;
                newUser.role = ADMIN;
                return newUser;
            case 3:
                // UserModel userModel = ;
                newUser.role = MANAGER;
                return newUser;
            case 4:
                // UserModel userModel = ;
                newUser.role = EMPLOYEE;
                return newUser;
            default:
                break;
            }
            }
}