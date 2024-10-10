#ifndef RESPONSE_MODEL_H
#define RESPONSE_MODEL_H

#include <stdio.h>

typedef struct responseModel
{
    int statusCode;
    char responseMessage[100];
    char serverMessage[100];
} ResponseModel;

#endif