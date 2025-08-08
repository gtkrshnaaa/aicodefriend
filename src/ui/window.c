#include "window.h"

// Implementasi fungsi untuk membuat jendela utama.
GtkWidget *window_new(AdwApplication *app) {
    // Membuat instance AdwApplicationWindow, ini adalah jendela khusus dari libadwaita
    // yang otomatis memberikan tampilan & nuansa GNOME.
    GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app));

    // Menetapkan judul jendela
    gtk_window_set_title(GTK_WINDOW(window), "AI CodeFriend");
    // Menetapkan ukuran default jendela
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 650);

    // --- Mulai Mendesain Isi Jendela ---

    // Membuat Header Bar (bar bagian atas khas aplikasi GNOME)
    GtkWidget *header = adw_header_bar_new();
    
    // Membuat layout utama menggunakan GtkBox vertikal
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Menambahkan Header Bar ke bagian atas main_box
    gtk_box_append(GTK_BOX(main_box), header);

    // Membuat label sebagai placeholder untuk area chat kita nanti
    GtkWidget *welcome_label = gtk_label_new("Area untuk chat akan muncul di sini...");
    gtk_widget_set_vexpand(welcome_label, TRUE); // Membuat label mengisi ruang vertikal

    // Menambahkan label ke dalam main_box
    gtk_box_append(GTK_BOX(main_box), welcome_label);

    // Menetapkan 'main_box' sebagai konten utama dari jendela
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), main_box);

    return window;
}