#include <stdio.h>
#include "../../models/user_auth_model.h"
#include "../../helper/auth_controller.h"

UserAuthModel printAdminMenu(UserModel user)
{
    UserAuthModel userAuthModel;
    UserModel newUser;

    printf("\n---Admin Menu Controls---\n");
    printf("1. Add New Customer/Employee\n");
    printf("2. Modify Customer/Employee Details\n");
    // printf("3. Manage User Roles\n");
    // printf("4. Change Password\n");
    printf("8. Logout\n");
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
        printf("8. Logout\n");
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
        case 8:
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
    }
    else if (choice == 1 && user.role == ADMIN)
    {
        printf("\n---Admin Menu Controls (Add New Customer/Employee)---\n");
        printf("1. Add an Manager\n");
        printf("2. Add an Employee\n");
        printf("3. Add an Customer\n");
        printf("8. Logout\n");
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
        case 8:
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
    }
    else if (choice == 2 && user.role == SUPERADMIN)
    {
        printf("\n---Admin Menu Controls (Modify Customer/Employee details)---\n");
        printf("1. Modify Admin Details\n");
        printf("2. Modify Manager Details\n");
        printf("3. Modify Employee Details\n");
        printf("4. Modify Customer Details\n");
        printf("8. Logout\n");
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
        case 8:
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
            userAuthModel.operation = ERROR;
            return userAuthModel;
        }

        // char username[100];
        // printf("Enter the correct username of the user you want to modify: ");
        // scanf("%s", username);

        // strcpy(newUser.username, username);
        // strcpy(newUser.password, "");
        // newUser.user_id = -1;
        // newUser.accStatus = ACTIVATED;
        // newUser.isLoggedIn = false;
        // userAuthModel.user = newUser;
        return userAuthModel;
    } else if (choice == 2 && user.role == ADMIN)
    {
        printf("\n---Admin Menu Controls (Modify Customer/Employee details)---\n");
        printf("1. Modify Manager Details\n");
        printf("2. Modify Employee Details\n");
        printf("3. Modify Customer Details\n");
        printf("8. Logout\n");
        printf("9. Exit\n");
        // int options;

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            userAuthModel.operation = MODIFY_MANAGER;
            break;
        case 2:
            userAuthModel.operation = MODIFY_EMPLOYEE;
            break;
        case 3:
            userAuthModel.operation = MODIFY_CUSTOMER;
            break;
        case 8:
            userAuthModel.operation = LOGOUT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        case 9:
            userAuthModel.operation = EXIT;
            userAuthModel.user = user;
            return userAuthModel;
            break;
        default:
            userAuthModel.operation = ERROR;
            return userAuthModel;
        }

        // char username[100];
        // printf("Enter the correct username of the user you want to modify: ");
        // scanf("%s", username);

        // strcpy(newUser.username, username);
        // strcpy(newUser.password, "");
        // newUser.user_id = -1;
        // newUser.accStatus = ACTIVATED;
        // newUser.isLoggedIn = false;
        // userAuthModel.user = newUser;
        return userAuthModel;
    }
    else if (choice == 8)
    {
        // printf("---here@admin_menu---");
        userAuthModel.operation = LOGOUT;
        user.accStatus = ACTIVATED;
        userAuthModel.user = user;
        // printUserModel(userAuthModel.user);
        return userAuthModel;
    }
    else if (choice == 9)
    {
        // printf("---here@admin_menu---");
        userAuthModel.operation = EXIT;
        user.accStatus = ACTIVATED;
        userAuthModel.user = user;
        // printUserModel(userAuthModel.user);
        return userAuthModel;
    } else {
        userAuthModel.operation = ERROR;
        return userAuthModel;
    }
}

UserModel modifyUserDetails()
{
    UserModel userModel;
    int userId;
    printf("\nEnter the id of the user you want to modify details of:");
    scanf("%d", &userId);
    userModel = getUserModelFromId(userId);
    printf("\n---Admin Menu Controls (Modify Customer/Employee details)---\n");
    printf("1. Modify User Role\n");
    printf("2. Change Password\n");
    printf("3. Activate/Deactivate\n");
    printf("8. Logout\n");
    printf("9. Exit\n");
    int options;

    printf("Enter your choice: ");
    scanf("%d", &options);

    switch (options)
    {
    case 1:
        char temp[100];
        strcpy(temp, userModel.username);
        strcpy(userModel.username, "dummy");
        ResponseModel response = updateUser(userModel.user_id, userModel);
        strcpy(userModel.username, temp);
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
        int create = createUser(userModel);
        userModel.user_id = -1;
        strcpy(userModel.username, "dummy");
        break;
    case 2:
        char password[100];
        printf("Change password to:\n");
        hidePasswordInput(password, sizeof(password));
        strcpy(userModel.password, password);
        break;
    case 3:
        printf("Press 1 to ACTIVATE and 2 to DEACTIVATE\n");
        scanf("%d", &options);
        userModel.accStatus = options == 1 ? ACTIVATED : DEACTIVATED;
    default:
        break;
    }

    return userModel;
}