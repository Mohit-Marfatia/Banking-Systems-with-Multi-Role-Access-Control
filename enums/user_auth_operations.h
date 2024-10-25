#ifndef USER_OPERATIONS_H
#define USER_OPERATIONS_H

typedef enum
{
    LOGIN,
    ADD_ADMIN,
    ADD_MANAGER,
    ADD_EMPLOYEE,
    ADD_CUSTOMER,
    MODIFY_ADMIN,
    MODIFY_MANAGER,
    MODIFY_EMPLOYEE,
    MODIFY_CUSTOMER,
    VIEW_BALANCE,
    DEPOSIT_MONEY,
    WITHDRAW_MONEY,
    TRANSACT_MONEY,
    VIEW_TRANSACTION_HISTORY,
    APPLY_LOAN,
    ADD_FEEDBACK,
    ASSIGN_LOAN_TO_EMPLOYEE,
    APPROVE_REJECT_LOAN,
    VIEW_FEEDBACK,
    CHANGE_PASSWORD,
    ERROR,
    LOGOUT,
    EXIT
} UserOperations;

const char *getOperationName(UserOperations opr)
{
    switch (opr)
    {
    case LOGIN:
        return "LOGIN";
    case MODIFY_CUSTOMER:
        return "MODIFY_CUSTOMER";
    case ADD_ADMIN:
        return "ADD_ADMIN";
    case ADD_MANAGER:
        return "ADD_MANAGER";
    case ADD_EMPLOYEE:
        return "ADD_EMPLOYEE";
    case ADD_CUSTOMER:
        return "ADD_CUSTOMER";
    case MODIFY_ADMIN:
        return "MODIFY_ADMIN";
    case MODIFY_MANAGER:
        return "MODIFY_MANAGER";
    case MODIFY_EMPLOYEE:
        return "MODIFY_EMPLOYEE";
    case VIEW_BALANCE:
        return "VIEW_BALANCE";
    case DEPOSIT_MONEY:
        return "DEPOSIT_MONEY";
    case WITHDRAW_MONEY:
        return "WITHDRAW_MONEY";
    case TRANSACT_MONEY:
        return "TRANSACT_MONEY";
    case VIEW_TRANSACTION_HISTORY:
        return "VIEW_TRANSACTION_HISTORY";
    case APPLY_LOAN:
        return "APPLY_LOAN";
    case ASSIGN_LOAN_TO_EMPLOYEE:
        return "ASSIGN_LOAN_TO_EMPLOYEE";
    case APPROVE_REJECT_LOAN:
        return "APPROVE_REJECT_LOAN";
    case ADD_FEEDBACK:
        return "ADD_FEEDBACK";
    case VIEW_FEEDBACK:
        return "VIEW_FEEDBACK";
    case CHANGE_PASSWORD:
        return "CHANGE_PASSWORD";
    case ERROR:
        return "ERROR";
    case LOGOUT:
        return "LOGOUT";
    case EXIT:
        return "EXIT";
    default:
        return "UNKNOWN";
    }
}

#endif