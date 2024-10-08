#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../models/user_model.h"
#include "../models/response_model.h"

// Function to lock file
int lockFile(int fd, int lockType) {
    struct flock fl;
    fl.l_type = lockType;  // Type of lock: F_RDLCK, F_WRLCK, F_UNLCK
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;  // Lock the whole file
    fl.l_len = 0;    // 0 means to lock the entire file
    return fcntl(fd, F_SETLKW, &fl);  // Apply the lock
}

// Function to create a user and append it to the file
int createUser(const char *filename, UserModel user) {
    int fd = open(filename, O_RDWR | O_CREAT, 0666);  // Open file for reading and writing
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }

    // Apply exclusive write lock
    if (lockFile(fd, F_WRLCK) == -1) {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    // Append user data to the file
    lseek(fd, 0, SEEK_END);  // Move to the end of the file
    if (write(fd, &user, sizeof(UserModel)) != sizeof(UserModel)) {
        perror("Error writing to file");
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    return 0;
}

// Function to read users from the file with shared lock
int readUsers(const char *filename) {
    int fd = open(filename, O_RDONLY);  // Open file for reading
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }

    // Apply shared read lock
    if (lockFile(fd, F_RDLCK) == -1) {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    UserModel user;
    printf("Users in the database:\n");
    while (read(fd, &user, sizeof(UserModel)) == sizeof(UserModel)) {
        printf("User ID: %d, Username: %s, Role: %s\n", user.user_id, user.username, getRoleName(user.role));
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    return 0;
}

// Function to get user-id from the file with shared lock
ResponseModel getUserId(const char *filename, UserModel userModel) {
    ResponseModel response;
    int fd = open(filename, O_RDONLY);  // Open file for reading
    if (fd < 0) {
        response.statusCode = 400;
        strcpy(response.responseMessage, "Error opening file\n");
        return response;
    }

    // Apply shared read lock
    if (lockFile(fd, F_RDLCK) == -1) {
        close(fd);
        response.statusCode = 400;
        strcpy(response.responseMessage, "Error locking file\n");
        return response;
    }

    UserModel user;
    int id = -1;
    printf("Users in the database:\n");
    while (read(fd, &user, sizeof(UserModel)) == sizeof(UserModel)) {
        if(user.username == userModel.username && user.password == userModel.password){
            id = userModel.user_id;
        }
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    if(id == -1){
        response.statusCode = 400;
        strcpy(response.responseMessage, "Either username or password is incorrect. Try again!\n");
    } else {
        response.statusCode = 200;
        strcpy(response.responseMessage, "Login successfull!\n");
    }
    
    return response;
}


// Function to update a user by user_id
int updateUser(const char *filename, int user_id, UserModel updatedUser) {
    int fd = open(filename, O_RDWR);  // Open file for reading and writing
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }

    // Apply exclusive write lock
    if (lockFile(fd, F_WRLCK) == -1) {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    UserModel user;
    while (read(fd, &user, sizeof(UserModel)) == sizeof(UserModel)) {
        if (user.user_id == user_id) {
            // Move the file pointer back to overwrite the user
            lseek(fd, -sizeof(UserModel), SEEK_CUR);
            if (write(fd, &updatedUser, sizeof(UserModel)) != sizeof(UserModel)) {
                perror("Error writing to file");
            }
            break;
        }
    }

    // Unlock the file
    lockFile(fd, F_UNLCK);

    close(fd);
    return 0;
}

// Function to delete a user by user_id
int deleteUser(const char *filename, int user_id) {
    int fd = open(filename, O_RDWR);  // Open file for reading and writing
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }

    // Apply exclusive write lock
    if (lockFile(fd, F_WRLCK) == -1) {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    FILE *tmpFile = fopen("temp.bin", "wb");  // Temporary file to store updated users
    if (!tmpFile) {
        perror("Error creating temp file");
        close(fd);
        return -1;
    }

    UserModel user;
    while (read(fd, &user, sizeof(UserModel)) == sizeof(UserModel)) {
        if (user.user_id != user_id) {
            fwrite(&user, sizeof(UserModel), 1, tmpFile);  // Copy all except the one to delete
        }
    }

    // Close both files
    fclose(tmpFile);
    close(fd);

    // Replace the original file with the temp file
    remove(filename);                // Delete the original file
    rename("temp.bin", filename);     // Rename temp file as the original

    return 0;
}

#endif