#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "../models/user_model.h"
#include "../models/account_model.h"

int lockRecordUserDb(int fd, int id, int lock_type)
{
    struct flock fl;
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = id * sizeof(UserModel);
    fl.l_len = sizeof(UserModel);
    fl.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &fl);
}

int lockRecordDbInfo(int fd, int lock_type)
{
    struct flock fl;
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &fl);
}

int lockAccountDb(int fd, int lock_type)
{
    struct flock fl;
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

      int result = fcntl(fd, F_SETLKW, &fl);
    if (result == -1) {
        perror("Error during fcntl");
    }
    return result;
}

int lockTransactionDb(int fd, int lock_type)
{
    struct flock fl;
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    return fcntl(fd, F_SETLKW, &fl);
}

int lockLoanDb(int fd, int lock_type)
{
    struct flock fl;
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    return fcntl(fd, F_SETLKW, &fl);
}

int lockFeedbackDb(int fd, int lockType)
{
    struct flock lock;
    lock.l_type = lockType;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    return fcntl(fd, F_SETLKW, &lock);
}

void hidePasswordInput(char *password, int size)
{
    struct termios oldt, newt;

    // Get the terminal settings for stdin
    tcgetattr(STDIN_FILENO, &oldt);

    // Set the new terminal settings (turn off echo)
    newt = oldt;
    newt.c_lflag &= ~(ECHO); // Disable echoing

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Get the password input
    printf("Enter password: ");
    scanf("%s", password);

    // Restore the old terminal settings (turn echo back on)
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n"); // Move to a new line after password input
}

void printMessage(char *errorMessage)
{
    printf("%s", errorMessage);
}

#endif