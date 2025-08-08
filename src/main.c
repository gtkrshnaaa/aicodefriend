#include "core/app.h"

int main(int argc, char **argv) {
    // Membuat instance aplikasi lengkap kita
    AICodeFriendApp *app = app_new();
    
    // Menjalankan aplikasi
    int status = g_application_run(G_APPLICATION(app->parent), argc, argv);
    
    // Membersihkan memori
    app_free(app);
    
    return status;
}