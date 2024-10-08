#ifndef USER_MODEL_H
#define USER_MODEL_H


#include <stdio.h>
#include <string.h>
#include "../enums/user_role.h"

typedef struct userModel {
    int user_id;
    UserRole role;
    char username[100];
    char password[100];
} UserModel;

#endif 