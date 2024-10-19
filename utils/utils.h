#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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