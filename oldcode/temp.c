#include "helper/auth_controller.h"
#include "models/user_model.h"
#include <string.h>
#include "definitions.h"

int main()
{
    const char *filename = "db/user_database.bin";
    
    // Example users
//     UserModel user1;

//     user1.user_id = 0;
//     user1.role = SUPERADMIN;
//     strcpy(user1.username, "super_admin");
//     strcpy(user1.password, "Abcd123$");
//     user1.isLoggedIn = false;
//     user1.accStatus = ENABLED;

//     // Create (Add users)
//     createUser(filename, user1);
//     // updateUser(filename, 0, user1);

//     for(int i = 1; i<= 500; i++){
//         // printf("%d ", i);
//         user1.user_id = i;
//         if(i >= ADMIN_ID_MIN && i <= ADMIN_ID_MAX){
//                 user1.role = ADMIN;
//         } else if(i >= MANAGER_ID_MIN && i <= MANAGER_ID_MAX){
//                 user1.role = MANAGER;
//         } else if(i >= EMPLOYEE_ID_MIN && i <= EMPLOYEE_ID_MAX){
//                 user1.role = EMPLOYEE;
//         } else if(i >= CUSTOMER_ID_MIN){
//                 user1.role = CUSTOMER;
//         }
//         strcpy(user1.username, "dummy");
//         strcpy(user1.password, "dummy");
//         user1.isLoggedIn = false;
//         user1.accStatus = DISABLED;

//         createUser(filename, user1);
//     }

    // readUsers(filename);

}