#ifndef ACCOUNT_TYPE_H
#define ACCOUNT_TYPE_H

typedef enum
{
    SAVINGS,
    LOAN
} AccountType;

const char *getAccountType(AccountType type)
{
    switch (type)
    {
    case SAVINGS:
        return "SAVINGS";
    case LOAN:
        return "LOAN";
    }
}

#endif