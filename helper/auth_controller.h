#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../models/user_model.h"
#include "../models/user_auth_model.h"
#include "../models/response_model.h"
#include "../definitions.h"

// Function to lock file
int lockFile(int fd, int lockType)
{
    struct flock fl;
    fl.l_type = lockType; // Type of lock: F_RDLCK, F_WRLCK, F_UNLCK
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;                  // Lock the whole file
    fl.l_len = 0;                    // 0 means to lock the entire file
    return fcntl(fd, F_SETLKW, &fl); // Apply the lock
}

// Function to create a user and append it to the file
int createUser(const char *filename, UserModel user)
{
    int fd = open(filename, O_RDWR | O_CREAT, 0666); // Open file for reading and writing
    if (fd < 0)
    {
        perror("Error opening file");
        return -1;
    }

    // Apply exclusive write lock
    if (lockFile(fd, F_WRLCK) == -1)
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    // Append user data to the file
    lseek(fd, 0, SEEK_END); // Move to the end of the file
    if (write(fd, &user, sizeof(UserModel)) != sizeof(UserModel))
    {
        perror("Error writing to file");
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    return 0;
}

ResponseModel createNewUser(const char *filename, UserAuthModel userAuthModel)
{
    ResponseModel response;
    UserModel userModel = userAuthModel.user;
    int fd = open(filename, O_RDWR); // Open file for reading
    if (fd < 0)
    {
        response.statusCode = 400;
        strcpy(response.responseMessage, "Error opening file\n");
        return response;
    }

    // Apply shared read lock
    if (lockFile(fd, F_RDLCK) == -1)
    {
        close(fd);
        response.statusCode = 400;
        strcpy(response.responseMessage, "Error locking file\n");
        return response;
    }

    UserModel user;
    int id = -1, maxId;
    if (userAuthModel.operation == ADD_ADMIN)
    {
        lseek(fd, ADMIN_ID_MIN * sizeof(UserModel), SEEK_SET);
        maxId = ADMIN_ID_MIN - 1;
    }
    else if (userAuthModel.operation == ADD_MANAGER)
    {
        lseek(fd, MANAGER_ID_MIN * sizeof(UserModel), SEEK_SET);
        maxId = MANAGER_ID_MIN - 1;
    }
    else if (userAuthModel.operation == ADD_EMPLOYEE)
    {
        lseek(fd, EMPLOYEE_ID_MIN * sizeof(UserModel), SEEK_SET);
        maxId = EMPLOYEE_ID_MIN - 1;
    }
    else if (userAuthModel.operation == ADD_CUSTOMER)
    {
        lseek(fd, CUSTOMER_ID_MIN * sizeof(UserModel), SEEK_SET);
        maxId = CUSTOMER_ID_MIN - 1;
    }

    // TODO: check for same username
    while (read(fd, &user, sizeof(user)))
    {
        int id = user.user_id;
        if (userAuthModel.operation == ADD_ADMIN)
        {
            if (strcmp(user.username, "dummy") != 0 && id <= ADMIN_ID_MAX)
            {
                printf("%d\n", id);
                maxId = id;
            }
            else
            {
                break;
            }
        }
        else if (userAuthModel.operation == ADD_MANAGER)
        {
            if (strcmp(user.username, "dummy") != 0 && id <= MANAGER_ID_MAX)
            {
                printf("%d\n", id);
                maxId = id;
            }
            else
            {
                break;
            }
        }
        else if (userAuthModel.operation == ADD_EMPLOYEE)
        {
            if (strcmp(user.username, "dummy") != 0 && id <= EMPLOYEE_ID_MAX)
            {
                printf("%d\n", id);
                maxId = id;
            }
            else
            {
                break;
            }
        }
        else if (userAuthModel.operation == ADD_CUSTOMER)
        {
            if (strcmp(user.username, "dummy") != 0)
            {
                printf("%d\n", id);
                maxId = id;
            }
            else
            {
                break;
            }
        }
    }

    if (userAuthModel.operation != ADD_CUSTOMER)
        lseek(fd, -1 * sizeof(UserModel), SEEK_CUR);
    userModel.user_id = maxId + 1;
    userModel.isLoggedIn = false;
    userModel.accStatus = ENABLED;
    write(fd, &userModel, sizeof(UserModel));
    // Unlock the file
    lockFile(fd, F_UNLCK);

    response.statusCode = 200;
    char temp[100];
    sprintf(temp, "\nUser added successfully! Id assigned = %d\n", maxId + 1);
    strcpy(response.responseMessage, temp);

    close(fd);
    return response;
}

// Function to read users from the file with shared lock
int readUsers(const char *filename)
{
    int fd = open(filename, O_RDONLY); // Open file for reading
    if (fd < 0)
    {
        perror("Error opening file");
        return -1;
    }

    // Apply shared read lock
    if (lockFile(fd, F_RDLCK) == -1)
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    UserModel user;
    printf("Users in the database:\n");
    while (read(fd, &user, sizeof(user)))
    {
        printUserModel(user);
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    return 0;
}

// Function to get user-id from the file with shared lock
ResponseModel getUserId(const char *filename, UserAuthModel userAuthModel)
{
    ResponseModel response;
    UserModel userModel = userAuthModel.user;
    int fd = open(filename, O_RDONLY); // Open file for reading
    if (fd < 0)
    {
        response.statusCode = 400;
        strcpy(response.responseMessage, "Error opening file\n");
        return response;
    }

    // Apply shared read lock
    if (lockFile(fd, F_RDLCK) == -1)
    {
        close(fd);
        response.statusCode = 400;
        strcpy(response.responseMessage, "Error locking file\n");
        return response;
    }

    UserModel user;
    int id = -1;
    bool isLoggedIn = false;
    while (read(fd, &user, sizeof(user)))
    {
        if (userAuthModel.operation == LOGIN)
        {
            if (strcmp(user.username, userModel.username) == 0 && strcmp(user.password, userModel.password) == 0)
            {
                // printf("\nfound user\n");
                id = user.user_id;
                isLoggedIn = user.isLoggedIn;
                break;
            }
        }
        else
        {   
    printf("------------debug--------------\n");
    printf("%s | %s\n", user.username, userModel.username);
            if (strcmp(user.username, userModel.username) == 0)
            {
                id = user.user_id;
                userModel.user_id = user.user_id;
                userModel.accStatus = user.accStatus;
                userModel.isLoggedIn = user.isLoggedIn;
                strcpy(userModel.password, user.password);
                userModel.role = user.role;
                strcpy(userModel.username, user.username);
                break;
            }
        }
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    if (id == -1)
    {
        response.statusCode = 400;
        strcpy(response.responseMessage, userAuthModel.operation == LOGIN ? "Either username or password is incorrect. Try again!\n" : "Username is incorrect. Try again!\n");
    }
    else if (userAuthModel.operation == LOGIN && isLoggedIn)
    {
        response.statusCode = 400;
        strcpy(response.responseMessage, "Maximum amount of logins reached! Please logout from other devices\n");
    }
    else
    {
        char str[5]; // Ensure the array is large enough to hold the number

        sprintf(str, "%d", id); // Convert int to string
        response.statusCode = 200;
        userAuthModel.operation == LOGIN ? strcpy(response.serverMessage, str) : userModelToString(userModel, response.serverMessage);
        strcpy(response.responseMessage, "Login successfull!\n");
    }

    return response;
}

// Function to update a user by user_id
ResponseModel updateUser(const char *filename, int user_id, UserModel updatedUser)
{
    ResponseModel responseModel;
    int fd = open(filename, O_RDWR); // Open file for reading and writing
    if (fd < 0)
    {
        responseModel.statusCode = 400;
        strcpy(responseModel.responseMessage, "Error opening file");
        return responseModel;
    }

    // Apply exclusive write lock
    if (lockFile(fd, F_WRLCK) == -1)
    {
        responseModel.statusCode = 400;
        strcpy(responseModel.responseMessage, "Error locking file");
        return responseModel;
    }

    UserModel user;
    // printf("------here------");
    if (user_id == 0)
    {
        lseek(fd, 0, SEEK_SET);
    }
    else if (user_id > 0 && user_id <= ADMIN_ID_MAX)
    {
        lseek(fd, ADMIN_ID_MIN * sizeof(UserModel), SEEK_SET);
    }
    else if (user_id <= MANAGER_ID_MAX)
    {
        lseek(fd, MANAGER_ID_MIN * sizeof(UserModel), SEEK_SET);
    }
    else if (user_id <= EMPLOYEE_ID_MAX)
    {
        lseek(fd, EMPLOYEE_ID_MIN * sizeof(UserModel), SEEK_SET);
    }
    else if (user_id >= CUSTOMER_ID_MIN)
    {
        lseek(fd, CUSTOMER_ID_MIN * sizeof(UserModel), SEEK_SET);
    }
    int count = 0;
    while (read(fd, &user, sizeof(user)))
    {
        if (user.user_id == user_id)
        {
            // Move the file pointer back to overwrite the user
            lseek(fd, -1 * sizeof(UserModel), SEEK_CUR);
            if (write(fd, &updatedUser, sizeof(UserModel)) != sizeof(UserModel))
            {
                responseModel.statusCode = 400;
                strcpy(responseModel.responseMessage, "Error writing to file");
                return responseModel;
            }
            break;
        }
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    responseModel.statusCode = 200;
    strcpy(responseModel.responseMessage, "Upadated user!");
    return responseModel;
}

// Function to delete a user by user_id
int deleteUser(const char *filename, int user_id)
{
    int fd = open(filename, O_RDWR); // Open file for reading and writing
    if (fd < 0)
    {
        perror("Error opening file");
        return -1;
    }

    // Apply exclusive write lock
    if (lockFile(fd, F_WRLCK) == -1)
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    FILE *tmpFile = fopen("temp.bin", "wb"); // Temporary file to store updated users
    if (!tmpFile)
    {
        perror("Error creating temp file");
        close(fd);
        return -1;
    }

    UserModel user;
    while (read(fd, &user, sizeof(user)))
    {
        if (user.user_id != user_id)
        {
            fwrite(&user, sizeof(UserModel), 1, tmpFile); // Copy all except the one to delete
        }
    }

    // Close both files
    fclose(tmpFile);
    close(fd);

    // Replace the original file with the temp file
    remove(filename);             // Delete the original file
    rename("temp.bin", filename); // Rename temp file as the original

    return 0;
}

#endif