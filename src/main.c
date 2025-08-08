#include "core/app.h"

int main(int argc, char **argv) {
    // Create the application instance
    AICodeFriendApp *app = app_new();
    
    // Run the application
    int status = g_application_run(G_APPLICATION(app->parent), argc, argv);
    
    // Free memory
    app_free(app);
    
    return status;
}