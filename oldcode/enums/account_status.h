typedef enum
{
    ENABLED, // 0
    DISABLED // 1
} AccountStatus;

const char *getAccountStatus(AccountStatus status)
{
    switch (status)
    {
    case ENABLED:
        return "ENABLED";
    case DISABLED:
        return "DISABLED";
    }
}