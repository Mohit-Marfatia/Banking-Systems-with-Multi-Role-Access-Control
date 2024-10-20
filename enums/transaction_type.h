#ifndef TRANSACTION_TYPE_H
#define TRANSACTION_TYPE_H

typedef enum
{
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER
} TransactionType;

const char *getTransactionType(TransactionType type)
{
    switch (type)
    {
    case DEPOSIT:
        return "DEPOSIT";
    case WITHDRAWAL:
        return "WITHDRAWAL";
    case TRANSFER:
        return "TRANSFER";
    default:
        return "UNKNOWN";
    }
}

#endif