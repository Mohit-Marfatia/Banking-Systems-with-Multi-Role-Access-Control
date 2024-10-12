typedef enum
{
    LOGIN,
    ADD_ADMIN,
    ADD_MANAGER,
    ADD_EMPLOYEE,
    ADD_CUSTOMER,
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
    case LOGOUT:
        return "LOGOUT";
    case EXIT:
        return "EXIT";
    default:
        return "UNKNOWN";
    }
}