typedef enum
{
    ACTIVATED, // 0
    DEACTIVATED // 1
} AccountStatus;

const char *getAccountStatus(AccountStatus status)
{
    switch (status)
    {
    case ACTIVATED:
        return "ACTIVATED";
    case DEACTIVATED:
        return "DEACTIVATED";
    }
}