#include "core/app.h"

int main(int argc, char **argv) {
    // Membuat instance aplikasi baru dengan memanggil fungsi dari modul app
    AdwApplication *app = app_new();
    
    // Menjalankan aplikasi dan menunggu hingga ditutup
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    // Membersihkan memori setelah aplikasi selesai
    g_object_unref(app);
    
    return status;
}
