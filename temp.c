#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "models/user_model.h"
#include "models/user_information_model.h"
#include "helper/auth_controller.h"

int main() {
    
    // UserInformationModel model;
    // model.adminCount = 0;
    // model.managerCount = 0;
    // model.employeeCount = 0;
    // model.customerCount = 0;
    // model.totalUsers = 0;

    // int fd = open(userInformationDatabase, O_RDWR | O_CREAT, 0666), id;
    // lseek(fd, 0, SEEK_SET);
    // write(fd, &model, sizeof(model));
    // close(fd);
    // UserModel user;
    // user.user_id = -1;
    // user.accStatus = ENABLED;
    // user.role = SUPERADMIN;
    // strcpy(user.username, "su");
    // strcpy(user.password, "0");
    // user.isLoggedIn = false;

    // createUser(user);

    // user.user_id = -1;
    // user.accStatus = ENABLED;
    // user.role = ADMIN;
    // strcpy(user.username, "admin1");
    // strcpy(user.password, "admin1");
    // user.isLoggedIn = false;

    // createUser(user);

    // readAllUsers();
    readAllManagers();
    return 0;
}