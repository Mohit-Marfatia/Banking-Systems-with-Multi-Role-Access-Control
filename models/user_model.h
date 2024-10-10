#ifndef USER_MODEL_H
#define USER_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../enums/user_role.h"
#include "../enums/account_status.h"

typedef struct userModel
{
    int user_id;
    UserRole role;
    char username[100];
    char password[100];
    bool isLoggedIn;
    AccountStatus accStatus;
} UserModel;

void printUserModel(UserModel user)
{
    printf("\nID: %d\n", user.user_id);
    printf("Role: %s\n", getRoleName(user.role));
    printf("Username: %s\n", user.username);
    printf("isLoggedIn: %s\n", user.isLoggedIn ? "true" : "false");
    printf("accStatus: %s\n", getAccountStatus(user.accStatus));
}
#endif