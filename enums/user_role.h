#ifndef USER_ROLE_H
#define USER_ROLE_H

typedef enum
{
    SUPERADMIN,
    ADMIN,
    MANAGER,
    EMPLOYEE,
    CUSTOMER,
    ALL 
} UserRole;

const char *getRoleName(UserRole role)
{
    switch (role)
    {
    case SUPERADMIN:
        return "SUPERADMIN";
    case ADMIN:
        return "ADMIN";
    case MANAGER:
        return "MANAGER";
    case EMPLOYEE:
        return "EMPLOYEE";
    case CUSTOMER:
        return "CUSTOMER";
    case ALL:
        return "ALL";
    default:
        return "UNKNOWN";
    }
}

#endif