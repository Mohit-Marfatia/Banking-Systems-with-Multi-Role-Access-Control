#ifndef DB_INFORMATION_MODEL_H
#define DB_INFORMATION_MODEL_H

typedef struct dbInformationModel
{
    int totalUsers;
    int adminCount;
    int managerCount;
    int employeeCount;
    int customerCount;
    int transactionsCount;
    int accountCount;
    int loanCount;
    int feedbackCount;
} DbInformationModel;

#endif