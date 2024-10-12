#ifndef DEFINITIONS_H // Header guard start
#define DEFINITIONS_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#define PORTNO 5050
#define ADMIN_ID_MIN 1
#define ADMIN_ID_MAX 9
#define MANAGER_ID_MIN 10
#define MANAGER_ID_MAX 99
#define EMPLOYEE_ID_MIN 100
#define EMPLOYEE_ID_MAX 499
#define CUSTOMER_ID_MIN 500

const char *enterUsername()
{
    return "Enter Username:\n";
}

const char *enterPassword()
{
    return "Enter Password:\n";
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