#ifndef USER_AUTH_MODEL_H
#define USER_AUTH_MODEL_H


#include <stdio.h>
#include <string.h>
#include "user_model.h"
#include "../enums/user_auth_operations.h"

typedef struct userAuthModel {
    UserModel user;
    UserAuthOperations opereation;
} UserAuthModel;

#endif 