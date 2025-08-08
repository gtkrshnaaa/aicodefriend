#include "window.h"
#include "chat_view.h" // Mengimpor modul chat view kita

// Deklarasi fungsi callback agar bisa ditemukan oleh gtk_builder atau g_signal_connect
static void on_send_button_clicked(GtkButton *button, gpointer user_data);

// Implementasi fungsi untuk membuat jendela utama.
GtkWidget *window_new(AdwApplication *app) {
    // Membuat instance AdwApplicationWindow, ini adalah jendela khusus dari libadwaita
    // yang otomatis memberikan tampilan & nuansa GNOME.
    GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app));

    // Menetapkan judul jendela
    gtk_window_set_title(GTK_WINDOW(window), "AI CodeFriend");
    // Menetapkan ukuran default jendela
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 700);

    // --- Mulai Mendesain Isi Jendela ---

    // Membuat Header Bar (bar bagian atas khas aplikasi GNOME)
    GtkWidget *header = adw_header_bar_new();
    
    // Membuat layout utama menggunakan GtkBox vertikal
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Menambahkan Header Bar ke bagian atas main_box
    gtk_box_append(GTK_BOX(main_box), header);

    // Memanggil modul chat_view untuk membuat area chat
    GtkWidget *chat_area = chat_view_new();

    // Menambahkan area chat ke dalam main_box
    gtk_box_append(GTK_BOX(main_box), chat_area);

    // Membuat 'input_bar' untuk menampung kolom input dan tombol kirim
    GtkWidget *input_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_start(input_bar, 12);
    gtk_widget_set_margin_end(input_bar, 12);
    gtk_widget_set_margin_top(input_bar, 6);
    gtk_widget_set_margin_bottom(input_bar, 12);
    
    // Membuat kolom input teks (GtkEntry)
    GtkWidget *text_entry = gtk_entry_new();
    gtk_widget_set_hexpand(text_entry, TRUE); // Buat agar mengisi sisa ruang horizontal
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Ketik pesanmu di sini...");

    // Membuat tombol kirim dengan ikon
    GtkWidget *send_button = gtk_button_new_from_icon_name("document-send-symbolic");
    gtk_widget_set_tooltip_text(send_button, "Kirim Pesan");

    // Menambahkan kolom input dan tombol ke dalam 'input_bar'
    gtk_box_append(GTK_BOX(input_bar), text_entry);
    gtk_box_append(GTK_BOX(input_bar), send_button);

    // Menambahkan 'input_bar' ke bagian bawah 'main_box'
    gtk_box_append(GTK_BOX(main_box), input_bar);
    
    // Menetapkan 'main_box' sebagai konten utama dari jendela
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), main_box);
    
    // --- Menghubungkan Sinyal ---
    // Menghubungkan sinyal 'clicked' dari tombol kirim ke fungsi callback
    g_signal_connect(send_button, "clicked", G_CALLBACK(on_send_button_clicked), NULL);

    return window;
}


// Implementasi fungsi callback untuk tombol kirim
static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
    // g_print adalah cara GTK untuk mencetak ke terminal, bagus untuk debugging.
    g_print("Tombol Kirim diklik!\n");

    // Di sinilah nanti kita akan memulai proses multi-threading.
    // 1. Ambil teks dari GtkEntry.
    // 2. Buat GTask untuk menjalankan request API di worker thread.
    // 3. Setelah selesai, GTask akan memanggil fungsi callback lain di main thread
    //    untuk menampilkan hasilnya di chat_view.
}