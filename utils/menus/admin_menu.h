#include <stdio.h>
#include "../../models/user_auth_model.h"
#include "../../definitions.h"

UserAuthModel printAdminMenu(UserModel user)
{
    UserAuthModel userAuthModel;
    UserModel newUser;

    printf("\n---Admin Menu Controls---\n");
    printf("1. Add New Bank Employee\n");
    printf("2. Modify Customer/Employee Details\n");
    printf("3. Manage User Roles\n");
    printf("4. Change Password\n");
    printf("5. Logout\n");
    printf("9. Exit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1 && user.role == SUPERADMIN)
    {
        printf("\n---Admin Menu Controls (Add New Bank Employee)---\n");
        printf("1. Add an Admin\n");
        printf("2. Add an Manager\n");
        printf("3. Add an Employee\n");
        printf("4. Add an Customer\n");
        printf("5. Logout\n");
        printf("9. Exit\n");
        int options;

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            userAuthModel.opereation = ADD_ADMIN;
            newUser.role = ADMIN;
            break;
        case 2:
            userAuthModel.opereation = ADD_MANAGER;
            newUser.role = MANAGER;
            break;
        case 3:
            userAuthModel.opereation = ADD_EMPLOYEE;
            newUser.role = EMPLOYEE;
            break;
        case 4:
            userAuthModel.opereation = ADD_CUSTOMER;
            newUser.role = CUSTOMER;
            break;
        case 5:
            userAuthModel.opereation = LOGOUT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        case 9:
            printf("---here@admin_menu---");
            userAuthModel.opereation = EXIT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        default:
            break;
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
    }
    else if (choice == 1 && user.role == ADMIN)
    {
        printf("\n---Admin Menu Controls (Add New Bank Employee)---\n");
        printf("1. Add an Manager\n");
        printf("2. Add an Employee\n");
        printf("3. Add an Customer\n");
        printf("4. Logout\n");
        printf("9. Exit\n");

        switch (choice)
        {
        case 1:
            userAuthModel.opereation = ADD_MANAGER;
            newUser.role = MANAGER;
            break;
        case 2:
            userAuthModel.opereation = ADD_EMPLOYEE;
            newUser.role = EMPLOYEE;
            break;
        case 3:
            userAuthModel.opereation = ADD_CUSTOMER;
            newUser.role = CUSTOMER;
            break;
        case 4:
            userAuthModel.opereation = LOGOUT;
            newUser.role = CUSTOMER;
            break;
        case 9:
            userAuthModel.opereation = EXIT;
            newUser.role = CUSTOMER;
            break;
        default:
            break;
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
    }
    else if (choice == 5)
    {
        // printf("---here@admin_menu---");
        userAuthModel.opereation = LOGOUT;
        user.accStatus = ENABLED;
        userAuthModel.user = user;
        // printUserModel(userAuthModel.user);
        return userAuthModel;
    }
    else if (choice == 9)
    {
        // printf("---here@admin_menu---");
        userAuthModel.opereation = EXIT;
        user.accStatus = ENABLED;
        userAuthModel.user = user;
        // printUserModel(userAuthModel.user);
        return userAuthModel;
    }
}