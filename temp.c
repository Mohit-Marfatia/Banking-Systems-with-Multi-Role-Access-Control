#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "models/user_model.h"
#include "models/db_information_model.h"
#include "helper/auth_controller.h"
#include "helper/account_controller.h"
#include "helper/transaction_controller.h"

int main() {
    
    // DbInformationModel model;
    // model.adminCount = 0;
    // model.managerCount = 0;
    // model.employeeCount = 0;
    // model.customerCount = 0;
    // model.totalUsers = 0;
    // model.transactionsCount = 0;
    // model.accountCount = 0;
    // model.loanCount = 0;
    // model.feedbackCount = 0;

    // int fd = open(dbInformationDatabase, O_RDWR | O_CREAT, 0666), id;
    // lseek(fd, 0, SEEK_SET);
    // write(fd, &model, sizeof(model));
    // close(fd);
    // UserModel user;
    // user.user_id = -1;
    // user.accStatus = ACTIVATED;
    // user.role = SUPERADMIN;
    // strcpy(user.username, "su");
    // strcpy(user.password, "0");
    // user.isLoggedIn = false;

    // createUser(user);

    // user.user_id = -1;
    // user.accStatus = ACTIVATED;
    // user.role = CUSTOMER;
    // strcpy(user.username, "cus1");
    // strcpy(user.password, "cus1");
    // user.isLoggedIn = false;

    // createUser(user);

    // user.user_id = -1;
    // user.accStatus = ACTIVATED;
    // user.role = CUSTOMER;
    // strcpy(user.username, "cus2");
    // strcpy(user.password, "cus2");
    // user.isLoggedIn = false;

    // createUser(user);

    // readAllUsers();
    // readAllAdmin();
    // readAllManagers();
    // readAllEmployees();
    // readAllCustomers();
    // readAllAccounts();
    readAllTransaction();
    // printf("\n");
    // printf("%d", getAccountIdFromUserId(1, SAVINGS));
    // printf("\n");
    // char *accountStr = readAccountsOfUserId(1);
    // printf("%s\n", accountStr);
    return 0;
}