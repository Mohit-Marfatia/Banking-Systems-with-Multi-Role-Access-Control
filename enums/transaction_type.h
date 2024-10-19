#ifndef TRANSACTION_TYPE_H
#define TRANSACTION_TYPE_H

typedef enum
{
    DEPOSIT,
    WITHDRAWAL
} TransactionType;

const char *getTransactionType(TransactionType type)
{
    switch (type)
    {
    case DEPOSIT:
        return "DEPOSIT";
    case WITHDRAWAL:
        return "WITHDRAWAL";
    }
}

#endif