#ifndef CUSTOMER_RESPONSE_MODEL_H
#define CUSTOMER_RESPONSE_MODEL_H

#include <stdio.h>
#include <string.h>
#include "user_model.h"
#include "../enums/user_auth_operations.h"

typedef struct customerResponseModel
{
    char customerResponse[100];
    UserOperations operation;
} CustomerResponseModel;

#endif