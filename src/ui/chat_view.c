#include "chat_view.h"

GtkWidget *chat_view_new(void) {
    // 1. Buat Scrolled Window agar area chat bisa di-scroll
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE); // Buat agar mengisi sisa ruang vertikal

    // 2. Buat Box sebagai 'lembaran kertas' untuk menampung semua pesan
    GtkWidget *message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    // Beri sedikit padding di pinggir agar tidak terlalu mepet
    gtk_widget_set_margin_start(message_box, 12);
    gtk_widget_set_margin_end(message_box, 12);
    gtk_widget_set_margin_top(message_box, 12);
    gtk_widget_set_margin_bottom(message_box, 12);

    // 3. Masukkan 'message_box' ke dalam 'scrolled_window'
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), message_box);

    // -- Tambahkan pesan selamat datang untuk tes --
    GtkWidget *welcome_label = gtk_label_new("Selamat datang di AI CodeFriend! ✨");
    // Atur agar label tidak di tengah, tapi di awal (kiri)
    gtk_widget_set_halign(welcome_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(message_box), welcome_label);
    
    // Kembalikan Scrolled Window sebagai widget utama dari modul ini
    return scrolled_window;
}