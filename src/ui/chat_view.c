#include "chat_view.h"
#include <gtksourceview/gtksource.h>

void chat_view_add_message(ChatView *chat_view, const gchar *text, ChatMessageType type) {
    // Membuat 'bubble' sebagai GtkBox agar bisa mengatur alignment dan properti lainnya
    GtkWidget *bubble_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    
    // Memberi style pada label
    gtk_widget_add_css_class(label, "bubble");

    if (type == CHAT_MESSAGE_USER) {
        gtk_widget_set_halign(bubble_box, GTK_ALIGN_END); // Rata kanan untuk user
        gtk_widget_add_css_class(label, "user-bubble");
    } else {
        gtk_widget_set_halign(bubble_box, GTK_ALIGN_START); // Rata kiri untuk AI
        gtk_widget_add_css_class(label, "ai-bubble");
    }

    // Masukkan label ke dalam bubble_box, ini mengatasi masalah max-width
    gtk_box_append(GTK_BOX(bubble_box), label);
    // Masukkan bubble_box ke message_box utama
    gtk_box_append(GTK_BOX(chat_view->message_box), bubble_box);
}

void chat_view_clear(ChatView *chat_view) {
    // Hapus semua anak dari message_box
    GtkWidget *child = gtk_widget_get_first_child(chat_view->message_box);
    while (child != NULL) {
        GtkWidget *next_child = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(chat_view->message_box), child);
        child = next_child;
    }
}


ChatView* chat_view_new(void) {
    ChatView *cv = g_new(ChatView, 1);

    cv->scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(cv->scrolled_window, TRUE);

    cv->message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(cv->message_box, 12);
    gtk_widget_set_margin_end(cv->message_box, 12);
    gtk_widget_set_margin_top(cv->message_box, 12);
    gtk_widget_set_margin_bottom(cv->message_box, 12);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(cv->scrolled_window), cv->message_box);

    chat_view_add_message(cv, "Hii, lets make some code.", CHAT_MESSAGE_AI);
    
    return cv;
}

void chat_view_free(ChatView *cv) {
    g_free(cv);
}