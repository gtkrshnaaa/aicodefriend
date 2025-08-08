#include "conversation.h"

// Fungsi custom untuk membersihkan memori ChatMessage secara lengkap
static void chat_message_free(gpointer data) {
    if (!data) return;
    ChatMessage *msg = (ChatMessage *)data;
    g_free(msg->role);
    g_free(msg->content);
    g_free(msg);
}

Conversation* conversation_new(void) {
    g_print("DEBUG: Creating new conversation\n");

    Conversation *conv = g_new(Conversation, 1);
    if (!conv) {
        g_printerr("ERROR: Failed to allocate Conversation\n");
        return NULL;
    }
    conv->history = g_ptr_array_new_with_free_func(chat_message_free);
    if (!conv->history) {
        g_printerr("ERROR: Failed to allocate history array\n");
        g_free(conv);
        return NULL;
    }
    return conv;
}

void conversation_add_message(Conversation *conv, const gchar *role, const gchar *content) {
    if (!conv || !role || !content) {
        g_printerr("ERROR: Invalid parameters in conversation_add_message\n");
        return;
    }

    g_print("DEBUG: Adding message to conversation: %s\n", content);

    ChatMessage *msg = g_new(ChatMessage, 1);
    if (!msg) {
        g_printerr("ERROR: Failed to allocate ChatMessage\n");
        return;
    }
    msg->role = g_strdup(role);
    msg->content = g_strdup(content);
    if (!msg->role || !msg->content) {
        g_printerr("ERROR: Failed to duplicate role or content\n");
        g_free(msg->role);
        g_free(msg->content);
        g_free(msg);
        return;
    }
    g_ptr_array_add(conv->history, msg);
}

void conversation_free(Conversation *conv) {
    if (!conv) {
        g_printerr("ERROR: Invalid conversation in conversation_free\n");
        return;
    }

    g_print("DEBUG: Freeing conversation\n");

    if (conv->history) {
        g_ptr_array_free(conv->history, TRUE);
    }
    g_free(conv);
}