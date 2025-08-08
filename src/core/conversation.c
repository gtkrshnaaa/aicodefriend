#include "conversation.h"
#include <glib.h>

// Forward declaration of chat_message_free
static void chat_message_free(ChatMessage *msg);

Conversation* conversation_new() {
    Conversation *conv = g_new(Conversation, 1);
    conv->history = g_ptr_array_new_with_free_func((GDestroyNotify)chat_message_free);
    g_print("DEBUG: Creating new conversation\n");
    return conv;
}

void conversation_add_message(Conversation *conv, const gchar *role, const gchar *content) {
    if (!conv || !role || !content) {
        g_printerr("ERROR: Invalid conversation, role, or content\n");
        return;
    }

    // Validate role
    if (g_strcmp0(role, "user") != 0 && g_strcmp0(role, "model") != 0) {
        g_printerr("ERROR: Invalid role '%s'. Must be 'user' or 'model'.\n", role);
        return;
    }

    ChatMessage *msg = g_new(ChatMessage, 1);
    msg->role = g_strdup(role);
    msg->content = g_strdup(content);
    g_ptr_array_add(conv->history, msg);
    g_print("DEBUG: Adding message to conversation: %s\n", content);
}

void chat_message_free(ChatMessage *msg) {
    if (msg) {
        g_free(msg->role);
        g_free(msg->content);
        g_free(msg);
    }
}

void conversation_free(Conversation *conv) {
    if (conv) {
        g_print("DEBUG: Freeing conversation\n");
        g_ptr_array_unref(conv->history);
        g_free(conv);
    }
}