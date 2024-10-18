#ifndef RESPONSE_MODEL_H
#define RESPONSE_MODEL_H

#include <stdio.h>

typedef struct responseModel
{
    int statusCode;
    char responseMessage[1024];
    char serverMessage[1024];
} ResponseModel;

#endif