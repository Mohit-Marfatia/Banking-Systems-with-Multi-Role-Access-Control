#include <stdio.h>
#include "../../models/user_auth_model.h"
#include "../../definitions.h"

UserAuthModel printAdminMenu(UserModel user)
{
    UserAuthModel userAuthModel;
    UserModel newUser;

    printf("\n---Admin Menu Controls---\n");
    printf("1. Add New Customer/Employee\n");
    printf("2. Modify Customer/Employee Details\n");
    // printf("3. Manage User Roles\n");
    // printf("4. Change Password\n");
    printf("3. Logout\n");
    printf("9. Exit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1 && user.role == SUPERADMIN)
    {
        printf("\n---Admin Menu Controls (Add New Customer/Employee)---\n");
        printf("1. Add an Admin\n");
        printf("2. Add an Manager\n");
        printf("3. Add an Employee\n");
        printf("4. Add an Customer\n");
        printf("5. Logout\n");
        printf("9. Exit\n");
        // int options;

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            userAuthModel.operation = ADD_ADMIN;
            newUser.role = ADMIN;
            break;
        case 2:
            userAuthModel.operation = ADD_MANAGER;
            newUser.role = MANAGER;
            break;
        case 3:
            userAuthModel.operation = ADD_EMPLOYEE;
            newUser.role = EMPLOYEE;
            break;
        case 4:
            userAuthModel.operation = ADD_CUSTOMER;
            newUser.role = CUSTOMER;
            break;
        case 5:
            userAuthModel.operation = LOGOUT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        case 9:
            // printf("---here@admin_menu---");
            userAuthModel.operation = EXIT;
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
        printf("\n---Admin Menu Controls (Add New Customer/Employee)---\n");
        printf("1. Add an Manager\n");
        printf("2. Add an Employee\n");
        printf("3. Add an Customer\n");
        printf("4. Logout\n");
        printf("9. Exit\n");
        // int options;

        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            userAuthModel.operation = ADD_MANAGER;
            newUser.role = MANAGER;
            break;
        case 2:
            userAuthModel.operation = ADD_EMPLOYEE;
            newUser.role = EMPLOYEE;
            break;
        case 3:
            userAuthModel.operation = ADD_CUSTOMER;
            newUser.role = CUSTOMER;
            break;
        case 4:
            userAuthModel.operation = LOGOUT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        case 9:
            // printf("---here@admin_menu---");
            userAuthModel.operation = EXIT;
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
    else if (choice == 2 && user.role == SUPERADMIN)
    {
        printf("\n---Admin Menu Controls (Modify Customer/Employee details)---\n");
        printf("1. Modify Admin Details\n");
        printf("2. Modify Manager Details\n");
        printf("3. Modify Employee Details\n");
        printf("4. Modify Customer Details\n");
        printf("5. Logout\n");
        printf("9. Exit\n");
        // int options;

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            userAuthModel.operation = MODIFY_ADMIN;
            break;
        case 2:
            userAuthModel.operation = MODIFY_MANAGER;
            break;
        case 3:
            userAuthModel.operation = MODIFY_EMPLOYEE;
            break;
        case 4:
            userAuthModel.operation = MODIFY_CUSTOMER;
            break;
        case 5:
            userAuthModel.operation = LOGOUT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        case 9:
            // printf("---here@admin_menu---");
            userAuthModel.operation = EXIT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        default:
            break;
        }

        char username[100];
        printf("Enter the correct username of the user you want to modify: ");
        scanf("%s", username);

        strcpy(newUser.username, username);
        strcpy(newUser.password, "");
        newUser.user_id = -1;
        newUser.accStatus = ENABLED;
        newUser.isLoggedIn = false;
        userAuthModel.user = newUser;
        return userAuthModel;
    }
    else if (choice == 3)
    {
        // printf("---here@admin_menu---");
        userAuthModel.operation = LOGOUT;
        user.accStatus = ENABLED;
        userAuthModel.user = user;
        // printUserModel(userAuthModel.user);
        return userAuthModel;
    }
    else if (choice == 9)
    {
        // printf("---here@admin_menu---");
        userAuthModel.operation = EXIT;
        user.accStatus = ENABLED;
        userAuthModel.user = user;
        // printUserModel(userAuthModel.user);
        return userAuthModel;
    }
}

UserModel modifyUserDetails(UserModel userModel)
{
    // int userModel;
    // userModel.user_id = userId;
    printf("\n---Admin Menu Controls (Modify Customer/Employee details)---\n");
    printf("1. Modify User Role\n");
    printf("2. Change Password\n");
    printf("5. Logout\n");
    printf("9. Exit\n");
    int options;

    printf("Enter your choice: ");
    scanf("%d", &options);

    switch (options)
    {
    case 1:
        printf("\n---Admin Menu Controls (Modify Customer/Employee details)---\n");
        printf(" Modify User Role to:\n");
        printf("1. Admin\n");
        printf("2. Manager\n");
        printf("3. Employee\n");
        printf("4. Customer\n");

        printf("Enter your choice: ");
        scanf("%d", &options);

        switch (options)
        {
        case 1:
            userModel.role = ADMIN;
            break;
        case 2:
            userModel.role = MANAGER;
            break;
        case 3:
            userModel.role = EMPLOYEE;
            break;
        case 4:
            userModel.role = CUSTOMER;
            break;
        default:
            break;
        }
        break;
    case 2:
        char password[100];
        printf("Change password to:\n");
        hidePasswordInput(password, sizeof(password));
        strcpy(userModel.password, password);
        break;
    default:
        break;
    }

    return userModel;
}