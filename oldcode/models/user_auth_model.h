#ifndef USER_AUTH_MODEL_H
#define USER_AUTH_MODEL_H

#include <stdio.h>
#include <string.h>
#include "user_model.h"
#include "../enums/user_auth_operations.h"

typedef struct userAuthModel
{
    UserModel user;
    UserAuthOperations operation;
} UserAuthModel;

// Convert UserAuthModel to string
void userAuthModelToString(UserAuthModel authModel, char *output)
{
    char userString[500];
    userModelToString(authModel.user, userString);

    sprintf(output, "%s,%d", userString, authModel.operation);
}

// Parse string to UserAuthModel
void userAuthModelFromString(const char *input, UserAuthModel *authModel)
{
    char userString[500];
    int operation;

    // Extract the user string and operation
    sscanf(input, "%499[^,],%d", userString, &operation);

    userModelFromString(userString, &authModel->user);
    authModel->operation = (UserAuthOperations)operation;
}
#endif