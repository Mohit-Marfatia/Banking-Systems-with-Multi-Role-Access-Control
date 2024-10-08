typedef enum {
    SUPERADMIN,  // 0
    ADMIN,       // 1
    MANAGER,     // 2
    EMPLOYEE,    // 3
    CUSTOMER     // 4
} UserRole;

const char* getRoleName(UserRole role) {
    switch (role) {
        case SUPERADMIN: return "SUPERADMIN";
        case ADMIN:      return "ADMIN";
        case MANAGER:    return "MANAGER";
        case EMPLOYEE:   return "EMPLOYEE";
        case CUSTOMER:   return "CUSTOMER";
        default:         return "UNKNOWN";
    }
}