#ifndef AICODEFRIEND_CONVERSATION_H
#define AICODEFRIEND_CONVERSATION_H

#include <glib.h>

typedef struct {
    gchar *role;
    gchar *content;
} ChatMessage;

typedef struct {
    GPtrArray *history;
} Conversation;

Conversation* conversation_new(void);
void conversation_add_message(Conversation *conv, const gchar *role, const gchar *content);
void conversation_free(Conversation *conv);

#endif //AICODEFRIEND_CONVERSATION_H