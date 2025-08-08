#ifndef AICODEFRIEND_APP_H
#define AICODEFRIEND_APP_H

#include <adwaita.h>
#include "../utils/config.h"
#include "../ui/chat_view.h" 
#include "conversation.h"

typedef struct _AICodeFriendApp {
    AdwApplication *parent;
    
    ConfigData *config;
    Conversation *conversation;

    // Widget Utama
    GtkWidget *main_window;
    ChatView *chat_view; 
    GtkWidget *send_button;
    GtkTextBuffer *text_entry_buffer;
    GtkWidget *history_list_box;

} AICodeFriendApp;

AICodeFriendApp *app_new(void);
void app_free(AICodeFriendApp *app);

#endif //AICODEFRIEND_APP_H