#ifndef AICODEFRIEND_APP_H
#define AICODEFRIEND_APP_H

#include <adwaita.h>
#include "../utils/config.h"
#include "conversation.h"

// Memberikan nama tag '_AICodeFriendApp' pada struct
typedef struct _AICodeFriendApp {
    AdwApplication *parent;
    
    // Data
    ConfigData *config;
    Conversation *conversation;

    // Widget Utama
    GtkWidget *main_window;
    GtkWidget *chat_view;
    GtkWidget *send_button;
    GtkTextBuffer *text_entry_buffer;

} AICodeFriendApp;

AICodeFriendApp *app_new(void);
void app_free(AICodeFriendApp *app);

#endif //AICODEFRIEND_APP_H