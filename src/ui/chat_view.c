#include "chat_view.h"
#include <gtksourceview/gtksource.h>

// Fungsi untuk mem-parsing dan menampilkan pesan
void chat_view_add_message(GtkWidget *chat_view, const gchar *text, ChatMessageType type) {
    // Mengambil message_box dari scrolled_window
    GtkWidget *scrolled_window = chat_view;
    GtkWidget *message_box = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(scrolled_window));

    // Membuat label dengan Pango Markup agar bisa diformat (bold, italic, dll)
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_label_set_wrap(GTK_LABEL(label), TRUE); // Agar teks panjang otomatis ke bawah
    gtk_label_set_xalign(GTK_LABEL(label), 0.0); // Rata kiri

    // Memberi style pada label agar terlihat seperti gelembung chat
    gtk_widget_set_valign(label, GTK_ALIGN_START);
    gtk_widget_add_css_class(label, "bubble");

    // Mengatur perataan berdasarkan tipe pesan
    if (type == CHAT_MESSAGE_USER) {
        gtk_widget_set_halign(label, GTK_ALIGN_END); // Rata kanan untuk user
        gtk_widget_add_css_class(label, "user-bubble");
    } else {
        gtk_widget_set_halign(label, GTK_ALIGN_START); // Rata kiri untuk AI
        gtk_widget_add_css_class(label, "ai-bubble");
    }

    // Menambahkan gelembung pesan ke dalam box
    gtk_box_append(GTK_BOX(message_box), label);
}


GtkWidget *chat_view_new(void) {
    // 1. Buat Scrolled Window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);

    // 2. Buat Box untuk menampung pesan
    GtkWidget *message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(message_box, 12);
    gtk_widget_set_margin_end(message_box, 12);
    gtk_widget_set_margin_top(message_box, 12);
    gtk_widget_set_margin_bottom(message_box, 12);

    // 3. Masukkan message_box ke dalam scrolled_window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), message_box);

    // Tambahkan pesan selamat datang
    chat_view_add_message(scrolled_window, "Halo, Kiann! Aku Caecillia, siap membantumu hari ini ✨", CHAT_MESSAGE_AI);
    
    return scrolled_window;
}