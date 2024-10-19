#ifndef LOAN_STATUS_H
#define LOAN_STATUS_H

typedef enum
{
    APPROVED,
    REJECTED,
    PENDING
} LoanStatus;

const char *getLoanStatus(LoanStatus type)
{
    switch (type)
    {
    case APPROVED:
        return "APPROVED";
    case REJECTED:
        return "REJECTED";
    case PENDING:
        return "PENDING";
    }
}

#endif