#include <stdio.h>
#include "../../models/user_model.h"
#include "../../models/user_auth_model.h"
#include "../../utils/utils.h"

UserAuthModel userLoginDetails()
{
    int choice;
    UserModel newUser;
    UserAuthModel userAuthModel;
    printf("\nLogin as:\n");
    printf("1. Customer\n");
    printf("2. Bank Employee\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        char username[100], password[100];
        printf("Enter username: ");
        scanf("%s", username);
        hidePasswordInput(password, sizeof(password));
        newUser.isLoggedIn = false;
        newUser.user_id = -1;
        newUser.role = CUSTOMER;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        userAuthModel.user = newUser;
        userAuthModel.operation = LOGIN;
        return userAuthModel;
    }
    else if (choice == 9)
    {

        newUser.user_id = -1;
        newUser.role = CUSTOMER;
        strcpy(newUser.username, "");
        strcpy(newUser.password, "");
        userAuthModel.user = newUser;
        userAuthModel.operation = EXIT;
        return userAuthModel;
    }
    else if (choice == 2)
    {
        printf("\n---Login as (Bank Employee):---\n");
        printf("1. Login as Super Admin\n");
        printf("2. Login as Admin\n");
        printf("3. Login as Manager\n");
        printf("4. Login as Employee\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            newUser.role = SUPERADMIN;
            userAuthModel.operation = LOGIN;
            break;
        case 2:
            newUser.role = ADMIN;
            userAuthModel.operation = LOGIN;
            break;
        case 3:
            newUser.role = MANAGER;
            userAuthModel.operation = LOGIN;
            break;
        case 4:
            newUser.role = EMPLOYEE;
            userAuthModel.operation = LOGIN;
            break;
        default:
            userAuthModel.operation = ERROR;
            return userAuthModel;
        }

        char username[100], password[100];
        printf("Enter username: ");
        scanf("%s", username);
        hidePasswordInput(password, sizeof(password));

        newUser.user_id = -1;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);

            userAuthModel.user = newUser;

            return userAuthModel;
    } else{
        userAuthModel.operation = ERROR;
        return userAuthModel;
    }
}