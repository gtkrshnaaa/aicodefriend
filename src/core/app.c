#include "app.h"
#include "../ui/window.h" // Perlu tahu tentang window_new()

// Ini adalah fungsi callback yang akan dijalankan saat aplikasi diaktifkan.
static void app_activate(GApplication *application) {
    // Membuat jendela utama kita dengan memanggil fungsi dari modul window
    GtkWidget *window = window_new(ADW_APPLICATION(application));
    
    // Menampilkan jendela yang sudah dibuat
    gtk_window_present(GTK_WINDOW(window));
}

// Implementasi fungsi untuk membuat instance aplikasi baru.
AdwApplication *app_new(void) {
    // Membuat instance AdwApplication dengan Application ID yang unik
    // Ini adalah standar untuk aplikasi GTK modern.
    AdwApplication *app = adw_application_new("dev.kiann.aicodefriend", G_APPLICATION_DEFAULT_FLAGS);
    
    // Menghubungkan sinyal "activate" dengan fungsi callback app_activate
    // Artinya, ketika aplikasi dijalankan, fungsi app_activate akan dipanggil.
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    
    return app;
}