#ifndef ACCOUNT_STATUS_H
#define ACCOUNT_STATUS_H

typedef enum
{
    ACTIVATED,
    DEACTIVATED
} AccountStatus;

const char *getAccountStatus(AccountStatus status)
{
    switch (status)
    {
    case ACTIVATED:
        return "ACTIVATED";
    case DEACTIVATED:
        return "DEACTIVATED";
    }
}

#endif