#include <stdio.h>
#include "../../models/user_model.h"
#include "../../models/user_auth_model.h"
#include "../../definitions.h"

UserAuthModel userLoginMenu()
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

        newUser.user_id = -1;
        newUser.role = CUSTOMER;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        userAuthModel.user = newUser;
        userAuthModel.opereation = LOGIN;
        return userAuthModel;
    }
    else if (choice == 9)
    {

        newUser.user_id = -1;
        newUser.role = CUSTOMER;
        strcpy(newUser.username, "");
        strcpy(newUser.password, "");
        userAuthModel.user = newUser;
        userAuthModel.opereation = EXIT;
        return userAuthModel;
    }
    else
    {
        printf("\n---Login as (Bank Employee):---\n");
        printf("1. Login as Super Admin\n");
        printf("2. Login as Admin\n");
        printf("3. Login as Manager\n");
        printf("4. Login as Employee\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        char username[100], password[100];
        printf("Enter username: ");
        scanf("%s", username);
        hidePasswordInput(password, sizeof(password));

        newUser.user_id = -1;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);

        switch (choice)
        {
        case 1:
            newUser.role = SUPERADMIN;
            userAuthModel.user = newUser;
            userAuthModel.opereation = LOGIN;
            return userAuthModel;
        case 2:
            newUser.role = ADMIN;
            userAuthModel.user = newUser;
            userAuthModel.opereation = LOGIN;
            return userAuthModel;
        case 3:
            newUser.role = MANAGER;
            userAuthModel.user = newUser;
            userAuthModel.opereation = LOGIN;
            return userAuthModel;
        case 4:
            newUser.role = EMPLOYEE;
            userAuthModel.user = newUser;
            userAuthModel.opereation = LOGIN;
            return userAuthModel;
        default:
            break;
        }
    }
}