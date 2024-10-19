#ifndef ACCOUNT_MODEL_H
#define ACCOUNT_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../enums/account_type.h"
#include "../enums/account_status.h"

typedef struct accountModel
{
    int account_id;
    int user_id;
    AccountType accType;
    int balance;
} AccountModel;

#endif