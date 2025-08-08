#ifndef AICODEFRIEND_CHAT_VIEW_H
#define AICODEFRIEND_CHAT_VIEW_H

#include <gtk/gtk.h>

typedef enum {
    CHAT_MESSAGE_USER,
    CHAT_MESSAGE_AI
} ChatMessageType;

typedef struct {
    GtkWidget *scrolled_window;
    GtkWidget *message_box;
} ChatView;

ChatView* chat_view_new(void);
void chat_view_add_message(ChatView *chat_view, const gchar *text, ChatMessageType type);
void chat_view_clear(ChatView *chat_view);
void chat_view_free(ChatView *chat_view);

#endif //AICODEFRIEND_CHAT_VIEW_H