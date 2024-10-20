#ifndef TRANSACTION_MODEL_H
#define TRANSACTION_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../enums/transaction_type.h"

typedef struct transactionModel
{
    int transaction_id;
    int account_id;
    int fromUserId;
    int toUserId;
    TransactionType transactionType;
    int amount;
} TransactionModel;

#endif