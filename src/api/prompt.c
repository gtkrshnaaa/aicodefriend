#include "prompt.h"
#include <json-glib/json-glib.h>
#include "../core/conversation.h"

gchar *prompt_build_json(ConfigData *config, Conversation *conv, const gchar *user_input) {
    if (!config || !conv || !user_input) {
        g_printerr("ERROR: Invalid config, conversation, or user_input\n");
        return NULL;
    }

    JsonBuilder *builder = json_builder_new();
    json_builder_begin_object(builder);

    // Add system context
    json_builder_set_member_name(builder, "system_instruction");
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "parts");
    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "text");
    json_builder_add_string_value(builder, config->system_context ? config->system_context : "");
    json_builder_end_object(builder);
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    // Add conversation history
    json_builder_set_member_name(builder, "contents");
    json_builder_begin_array(builder);
    
    for (guint i = 0; i < conv->history->len; i++) {
        ChatMessage *msg = g_ptr_array_index(conv->history, i);
        if (g_strcmp0(msg->role, "user") != 0 && g_strcmp0(msg->role, "model") != 0) {
            g_printerr("ERROR: Invalid role '%s' in conversation history\n", msg->role);
            continue;
        }
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "role");
        json_builder_add_string_value(builder, msg->role);
        json_builder_set_member_name(builder, "parts");
        json_builder_begin_array(builder);
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "text");
        json_builder_add_string_value(builder, msg->content ? msg->content : "");
        json_builder_end_object(builder);
        json_builder_end_array(builder);
        json_builder_end_object(builder);
    }

    // Add new user input
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "role");
    json_builder_add_string_value(builder, "user");
    json_builder_set_member_name(builder, "parts");
    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "text");
    json_builder_add_string_value(builder, user_input);
    json_builder_end_object(builder);
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    json_builder_end_array(builder);
    json_builder_end_object(builder);

    JsonNode *root = json_builder_get_root(builder);
    gchar *json = json_to_string(root, TRUE);
    json_node_unref(root);
    g_object_unref(builder);

    return json;
}