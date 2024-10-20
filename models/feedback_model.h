#ifndef FEEDBACK_MODEL_H
#define FEEDBACK_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../enums/account_type.h"
#include "../enums/loan_status.h"

typedef struct feedbackModel
{
    int feedback_id;
    int user_id;
    char feedback_message[1024];
} FeedbackModel;

#endif