#ifndef LOAN_MODEL_H
#define LOAN_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../enums/account_type.h"
#include "../enums/loan_status.h"

typedef struct loanModel
{
    int loan_id;
    int user_id;
    LoanStatus loanStatus;
    int loan_amount;
} LoanModel;

#endif