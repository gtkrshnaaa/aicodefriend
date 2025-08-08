#include "conversation.h"

Conversation* conversation_new(void) {
    Conversation *conv = g_new(Conversation, 1);
    conv->history = g_ptr_array_new_with_free_func((GDestroyNotify)g_free);
    return conv;
}

void conversation_add_message(Conversation *conv, const gchar *role, const gchar *content) {
    ChatMessage *msg = g_new(ChatMessage, 1);
    msg->role = g_strdup(role);
    msg->content = g_strdup(content);
    g_ptr_array_add(conv->history, msg);
}

void conversation_free(Conversation *conv) {
    g_ptr_array_free(conv->history, TRUE);
    g_free(conv);
}