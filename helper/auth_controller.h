#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "../models/user_model.h"
#include "../models/user_id_model.h"
#include "../models/db_information_model.h"
#include "../models/user_auth_model.h"
#include "../models/response_model.h"
#include "../utils/constants.h"
#include "../utils/utils.h"
#include "../enums/account_type.h"
#include "./account_controller.h"

ResponseModel getUserId(UserAuthModel userAuthModel)
{
    int fd, id = -1;
    char str[5];

    ResponseModel responseModel;
    UserIdModel user;
    UserModel userModel = userAuthModel.user;

    if (strcmp(userModel.username, "su") == 0)
    {
        id = 0;
        sprintf(str, "%d", id); // Convert int to string
        responseModel.statusCode = 200;
        strcpy(responseModel.responseMessage, str);
        return responseModel;
    }

    switch (userModel.role)
    {
    case SUPERADMIN:
        if (strcmp(userModel.username, "su") == 0)
        {
            id = 0;
            sprintf(str, "%d", id); // Convert int to string
            responseModel.statusCode = 200;
            strcpy(responseModel.responseMessage, str);
            return responseModel;
        }
        else
        {

            responseModel.statusCode = 400;
            strcpy(responseModel.responseMessage, "Username or password incorrect!");
            return responseModel;
        }
    case ADMIN:
        fd = open(adminDatabase, O_RDWR | O_CREAT, 0666);
        break;
    case MANAGER:
        fd = open(managerDatabase, O_RDWR | O_CREAT, 0666);
        break;
    case EMPLOYEE:
        fd = open(employeeDatabase, O_RDWR | O_CREAT, 0666);
        break;
    case CUSTOMER:
        fd = open(customerDatabase, O_RDWR | O_CREAT, 0666);
        break;
    default:
        break;
    }
    if (fd < 0)
    {
        strcpy(responseModel.responseMessage, "Error opening file");
        responseModel.statusCode = 400;
        return responseModel;
    }

    // printf("----@debug---\n");;
    // printf("%d | %s\n", userModel.user_id, userModel.username);
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {

        if (strcmp(user.username, userModel.username) == 0)
        {
            id = user.user_id;
            break;
        }
    }

    // printf("id---%d\n", id);

    if (id == -1)
    {
        strcpy(responseModel.responseMessage, "User does not exist.\n");
        responseModel.statusCode = 400;
    }
    else
    {
        sprintf(str, "%d", id); // Convert int to string
        responseModel.statusCode = 200;
        strcpy(responseModel.serverMessage, str);
    }
    return responseModel;
}

int getCustomerId(char *str){

    UserIdModel user;
    int fd = open(customerDatabase, O_RDWR | O_CREAT, 0666), id;
     if (fd < 0){
        perror("Error opening file");
        return -1;
    }
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {

        if (strcmp(user.username, str) == 0)
        {
            id = user.user_id;
            break;
        }
    }


    if (id == -1)
    {
        printf("Customer not found\n");
    }
    else
    {
        return id;
    }
}

int getUserIdInformation(UserRole role)
{
    int fd = open(dbInformationDatabase, O_RDONLY), id;
    DbInformationModel model;
    lockRecordDbInfo(fd, F_RDLCK);
    lseek(fd, 0, SEEK_SET);
    read(fd, &model, sizeof(model));

    switch (role)
    {
    case ALL:
        printf("total users:%d\n", model.totalUsers);
        return model.totalUsers;
    case ADMIN:
        return model.adminCount;
    case MANAGER:
        return model.managerCount;
    case EMPLOYEE:
        return model.employeeCount;
    case CUSTOMER:
        return model.customerCount;

    default:
        break;
    }
    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);
}

void updateUserInformation(UserRole role)
{
    DbInformationModel model;
    int fd = open(dbInformationDatabase, O_RDWR | O_CREAT, 0666), count, total;
    lockRecordDbInfo(fd, F_WRLCK);
    lseek(fd, 0, SEEK_SET);
    read(fd, &model, sizeof(model));

    switch (role)
    {
    case ADMIN:
        count = model.adminCount;
        model.adminCount = count + 1;
        break;
    case MANAGER:
        count = model.managerCount;
        model.managerCount = count + 1;
        break;
    case EMPLOYEE:
        count = model.employeeCount;
        model.employeeCount = count + 1;
        break;
    case CUSTOMER:
        count = model.customerCount;
        model.customerCount = count + 1;
        break;

    default:
        break;
    }

    count = model.totalUsers;
    model.totalUsers = count + 1;
    write(fd, &model, sizeof(model));
    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);
}

void addUserToSpecificDb(void *arg)
{
    UserModel *user = (UserModel *)arg;
}

void *task1(void *arg)
{
    UserModel *user = (UserModel *)arg;
    int fd = open(userDatabase, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        perror("Error opening file");
    } // File descriptor
    if (lockRecordUserDb(fd, user->user_id, F_WRLCK) == -1)
    {
        perror("Error locking file");
        close(fd);
    }

    // Append user data to the file
    lseek(fd, 0, SEEK_END);
    if (write(fd, user, sizeof(UserModel)) != sizeof(UserModel))
    {
        perror("Error writing to file");
    }

    lockRecordUserDb(fd, user->user_id, F_UNLCK);
    close(fd);

    return NULL;
}

// Second task: Update user information based on role
void *task2(void *arg)
{
    UserModel *user = (UserModel *)arg;

    updateUserInformation(user->role);
    return NULL;
}

// Third task: Add user to a specific database
void *task3(void *arg)
{
    UserModel *user = (UserModel *)arg;

    addUserToSpecificDb(&user);

    return NULL;
}

int createUser(UserModel user)
{
    user.user_id = getUserIdInformation(ALL);
    user.isLoggedIn = false;
    user.accStatus = ACTIVATED;

    // Open user database file
    int fd = open(userDatabase, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        perror("Error opening user database");
        return -1;
    }

    // Lock the record for writing
    if (lockRecordUserDb(fd, user.user_id, F_WRLCK) == -1)
    {
        perror("Error locking user database");
        close(fd);
        return -1;
    }

    // Append user data to the end of the file
    if (lseek(fd, 0, SEEK_END) == -1)
    {
        perror("Error seeking to end of user database");
        close(fd);
        return -1;
    }

    if (write(fd, &user, sizeof(UserModel)) != sizeof(UserModel))
    {
        perror("Error writing to user database");
        close(fd);
        return -1;
    }

    // Unlock and close user database file
    lockRecordUserDb(fd, user.user_id, F_UNLCK);
    close(fd);

    // Update the user information model
    DbInformationModel model;
    int fd2 = open(dbInformationDatabase, O_RDWR);
    lockRecordDbInfo(fd2, F_WRLCK);
    if (fd2 < 0)
    {
        perror("Error opening user information database");
        return -1;
    }

    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        perror("Error seeking in user information database");
        close(fd2);
        return -1;
    }

    if (read(fd2, &model, sizeof(DbInformationModel)) != sizeof(DbInformationModel))
    {
        perror("Error reading from user information database");
        close(fd2);
        return -1;
    }

    // Update the role-specific count and total user count
    switch (user.role)
    {
    case ADMIN:
        model.adminCount++;
        break;
    case MANAGER:
        model.managerCount++;
        break;
    case EMPLOYEE:
        model.employeeCount++;
        break;
    case CUSTOMER:
        model.customerCount++;
        break;
    default:
        break;
    }

    model.totalUsers++;

    // Write updated model back to the file
    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        perror("Error seeking to start of user information database");
        close(fd2);
        return -1;
    }

    if (write(fd2, &model, sizeof(DbInformationModel)) != sizeof(DbInformationModel))
    {
        perror("Error writing to user information database");
        close(fd2);
        return -1;
    }
    lockRecordDbInfo(fd2, F_UNLCK);
    close(fd2);

    // Add user to the specific role-based database
    int fd3;
    UserIdModel idModel;
    switch (user.role)
    {
    case ADMIN:
        fd3 = open(adminDatabase, O_RDWR | O_CREAT, 0666);
        break;
    case MANAGER:
        fd3 = open(managerDatabase, O_RDWR | O_CREAT, 0666);
        break;
    case EMPLOYEE:
        fd3 = open(employeeDatabase, O_RDWR | O_CREAT, 0666);
        break;
    case CUSTOMER:
        fd3 = open(customerDatabase, O_RDWR | O_CREAT, 0666);
        break;
    default:
        fd3 = -1;
        break;
    }

    if (fd3 < 0)
    {
        perror("Error opening role-specific database");
        return -1;
    }

    // Append user ID and username to the role-specific database
    if (lseek(fd3, 0, SEEK_END) == -1)
    {
        perror("Error seeking to end of role-specific database");
        close(fd3);
        return -1;
    }

    idModel.user_id = user.user_id;
    strcpy(idModel.username, user.username);

    if (write(fd3, &idModel, sizeof(UserIdModel)) != sizeof(UserIdModel))
    {
        perror("Error writing to role-specific database");
        close(fd3);
        return -1;
    }

    close(fd3);
    // Create Savings Account of customer
    if(user.role == CUSTOMER) createAccount(user.user_id, SAVINGS);
    return 0;
}

ResponseModel login(int userId, UserModel userModel)
{
    ResponseModel responseModel;
    UserModel user;
    int fd = open(userDatabase, O_RDWR);
    if (fd < 0)
    {
        strcpy(responseModel.responseMessage, "Error opening file");
        responseModel.statusCode = 400;
        close(fd);
        return responseModel;
    }
    if (lockRecordUserDb(fd, userId, F_WRLCK) == -1)
    {
        strcpy(responseModel.responseMessage, "Error locking file");
        responseModel.statusCode = 400;
        close(fd);
        return responseModel;
    }
    
    // printUserModel(userModel);
    // Append user data to the file
    lseek(fd, userId * sizeof(UserModel), SEEK_SET);
    if (read(fd, &user, sizeof(UserModel)) != sizeof(UserModel))
    {
        strcpy(responseModel.responseMessage, "Error reading file");
        responseModel.statusCode = 400;
        close(fd);
        return responseModel;
    }

    lockRecordUserDb(fd, userId, F_UNLCK);
    close(fd);
    // printUserModel(user);
    // printf("----debug----\n");
    if (user.accStatus == DEACTIVATED)
    {

        responseModel.statusCode = 400;
        strcpy(responseModel.responseMessage, "Account has been disabled!");
         return responseModel;
    }
    else if (user.isLoggedIn == true)
    {

        responseModel.statusCode = 400;
        strcpy(responseModel.responseMessage, "Maximum amount of logins has been reached!");
         return responseModel;
    }
    else if (strcmp(user.password, userModel.password) == 0)
    {
        responseModel.statusCode = 200;
        strcpy(responseModel.responseMessage, "Login successful!");
         return responseModel;
    } else {
        responseModel.statusCode = 400;
        strcpy(responseModel.responseMessage, "Username or password incorrect. Try again!");
        return responseModel;
    }

    // return responseModel;
}

ResponseModel updateUser(int userId, UserModel userModel)
{
    ResponseModel responseModel;
    int fd = open(userDatabase, O_RDWR);
    if (fd < 0)
    {
        strcpy(responseModel.responseMessage, "Error opening file");
        responseModel.statusCode = 400;
        close(fd);
        return responseModel;
    }
    if (lockRecordUserDb(fd, userId, F_WRLCK) == -1)
    {
        strcpy(responseModel.responseMessage, "Error locking file");
        responseModel.statusCode = 400;
        close(fd);
        return responseModel;
    }

    // Append user data to the file
    lseek(fd, userId * sizeof(UserModel), SEEK_SET);
    if (write(fd, &userModel, sizeof(UserModel)) != sizeof(UserModel))
    {
        strcpy(responseModel.responseMessage, "Error writing to file");
        responseModel.statusCode = 400;
        close(fd);
        return responseModel;
    }

    lockRecordUserDb(fd, userId, F_UNLCK);
    close(fd);
    responseModel.statusCode = 200;
    strcpy(responseModel.responseMessage, "Upadated user!");
    return responseModel;
}

UserModel getUserModelFromId(int userId)
{
    UserModel userModel;
    int fd = open(userDatabase, O_RDWR);
    if (fd < 0)
    {
        close(fd);
    }
    if (lockRecordUserDb(fd, userId, F_WRLCK) == -1)
    {
        close(fd);
    }

    // Append user data to the file
    lseek(fd, userId * sizeof(UserModel), SEEK_SET);
    if (read(fd, &userModel, sizeof(UserModel)) != sizeof(UserModel))
    {
        close(fd);
    }

    lockRecordUserDb(fd, userId, F_UNLCK);
    close(fd);

    return userModel;
}
ResponseModel logout(int userId, UserModel userModel)
{
    ResponseModel responseModel;
    userModel.isLoggedIn = false;
    responseModel = updateUser(userId, userModel);
    responseModel.statusCode = 200;
    strcpy(responseModel.responseMessage, "Logout successful!");
    return responseModel;
}

void readAllUsers()
{
    UserModel user;
    int fd = open(userDatabase, O_RDONLY, 0600);
    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("Listing all users:\n");
    printf("\n%-10s %-20s %-15s %-10s %-10s\n", "ID", "Username", "Role", "LoggedIn", "Status");
    printf("-------------------------------------------------------------------\n");
    // Read the file record by record
    while (read(fd, &user, sizeof(UserModel)) == sizeof(UserModel))
    {
        if (user.user_id != -1)
        { // Only display records that are not marked as deleted

            printf("%-10d %-20s %-15s %-10s %-10s\n",
                   user.user_id,
                   user.username,
                   getRoleName(user.role),
                   user.isLoggedIn ? "true" : "false",
                   getAccountStatus(user.accStatus));
        }
    }
    close(fd);
}

void readAllAdmin()
{
    UserIdModel user;
    UserModel userModel;
    int fd = open(adminDatabase, O_RDONLY, 0600);
    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("Listing all admins:\n");
    printf("\n%-10s %-20s %-15s %-10s %-10s\n", "ID", "Username", "Role", "LoggedIn", "Status");
    printf("-------------------------------------------------------------------\n");
    // Read the file record by record
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {
        if (user.user_id != -1)
        { // Only display records that are not marked as deleted
            userModel = getUserModelFromId(user.user_id);
            printf("%-10d %-20s %-15s %-10s %-10s\n",
                   userModel.user_id,
                   userModel.username,
                   getRoleName(userModel.role),
                   userModel.isLoggedIn ? "true" : "false",
                   getAccountStatus(userModel.accStatus));
        }
    }

    close(fd);
}

void readAllManagers()
{
    UserIdModel user;
    UserModel userModel;
    int fd = open(managerDatabase, O_RDONLY, 0600);
    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("Listing all managers:\n");
    printf("\n%-10s %-20s %-15s %-10s %-10s\n", "ID", "Username", "Role", "LoggedIn", "Status");
    printf("-------------------------------------------------------------------\n");
    // Read the file record by record
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {
        if (user.user_id != -1)
        { // Only display records that are not marked as deleted

            userModel = getUserModelFromId(user.user_id);
            printf("%-10d %-20s %-15s %-10s %-10s\n",
                   userModel.user_id,
                   userModel.username,
                   getRoleName(userModel.role),
                   userModel.isLoggedIn ? "true" : "false",
                   getAccountStatus(userModel.accStatus));
        }
    }
    close(fd);
}

void readAllEmployees()
{
    UserIdModel user;
    UserModel userModel;
    int fd = open(employeeDatabase, O_RDONLY, 0600);
    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("Listing all emplyees:\n");
    printf("\n%-10s %-20s %-15s %-10s %-10s\n", "ID", "Username", "Role", "LoggedIn", "Status");
    printf("-------------------------------------------------------------------\n");
    // Read the file record by record
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {
        if (user.user_id != -1)
        { // Only display records that are not marked as deleted

            userModel = getUserModelFromId(user.user_id);
            printf("%-10d %-20s %-15s %-10s %-10s\n",
                   userModel.user_id,
                   userModel.username,
                   getRoleName(userModel.role),
                   userModel.isLoggedIn ? "true" : "false",
                   getAccountStatus(userModel.accStatus));
        }
    }
    close(fd);
}

void readAllCustomers()
{
    UserIdModel user;
    UserModel userModel;
    int fd = open(customerDatabase, O_RDONLY, 0600);
    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("Listing all customers:\n");
    printf("\n%-10s %-20s %-15s %-10s %-10s\n", "ID", "Username", "Role", "LoggedIn", "Status");
    printf("-------------------------------------------------------------------\n");
    // Read the file record by record
    while (read(fd, &user, sizeof(UserIdModel)) == sizeof(UserIdModel))
    {
        if (user.user_id != -1)
        { // Only display records that are not marked as deleted
            // printf("%d | %s\n", user.user_id, user.username);
            userModel = getUserModelFromId(user.user_id);
            printf("%-10d %-20s %-15s %-10s %-10s\n",
                   userModel.user_id,
                   userModel.username,
                   getRoleName(userModel.role),
                   userModel.isLoggedIn ? "true" : "false",
                   getAccountStatus(userModel.accStatus));
        }
    }
    close(fd);
}

#endif