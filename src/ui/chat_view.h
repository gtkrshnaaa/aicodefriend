#ifndef AICODEFRIEND_CHAT_VIEW_H
#define AICODEFRIEND_CHAT_VIEW_H

#include <gtk/gtk.h>

// Enum untuk membedakan pesan dari user atau dari AI
typedef enum {
    CHAT_MESSAGE_USER,
    CHAT_MESSAGE_AI
} ChatMessageType;

// Deklarasi fungsi untuk membuat widget chat view baru
GtkWidget *chat_view_new(void);

// Deklarasi fungsi untuk menambahkan pesan baru ke chat view
void chat_view_add_message(GtkWidget *chat_view, const gchar *text, ChatMessageType type);

#endif //AICODEFRIEND_CHAT_VIEW_H