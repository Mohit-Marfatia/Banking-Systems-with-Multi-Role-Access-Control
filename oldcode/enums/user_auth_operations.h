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
    LOGOUT,
    EXIT
} UserAuthOperations;

const char *getOperationName(UserAuthOperations opr)
{
    switch (opr)
    {
    case LOGIN:
        return "LOGIN";
    case ADD_ADMIN:
        return "ADD_ADMIN";
    case ADD_MANAGER:
        return "ADD_MANAGER";
    case ADD_EMPLOYEE:
        return "ADD_EMPLOYEE";
    case ADD_CUSTOMER:
        return "ADD_CUSTOMER";
    case MODIFY_ADMIN:
        return "ADD_ADMIN";
    case MODIFY_MANAGER:
        return "MODIFY_MANAGER";
    case MODIFY_EMPLOYEE:
        return "MODIFY_EMPLOYEE";
    case MODIFY_CUSTOMER:
        return "MODIFY_CUSTOMER";
    case LOGOUT:
        return "LOGOUT";
    case EXIT:
        return "EXIT";
    default:
        return "UNKNOWN";
    }
}