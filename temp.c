#include "helper/auth_controller.h"
#include "models/user_model.h"
#include <string.h>

int main() {
    const char *filename = "db/user_database.bin";

    // Example users
    UserModel user1;

    user1.user_id = 0;
    user1.role = SUPERADMIN;
    strcpy(user1.username , "super_admin");
    strcpy(user1.password , "Abcd123$");

    // Create (Add users)
    createUser(filename, user1);

    readUsers(filename);
}