#include "prompt.h"
#include <json-glib/json-glib.h>

gchar* prompt_build_json(ConfigData *config, Conversation *conversation, const gchar *current_input) {
    JsonBuilder *builder = json_builder_new();

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "contents");
    json_builder_begin_array(builder);

    // System Context
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "role");
    json_builder_add_string_value(builder, "system");
    json_builder_set_member_name(builder, "parts");
    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "text");
    json_builder_add_string_value(builder, config->system_context);
    json_builder_end_object(builder);
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    // AI Persona (Model)
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "role");
    json_builder_add_string_value(builder, "model");
    json_builder_set_member_name(builder, "parts");
    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "text");
    json_builder_add_string_value(builder, config->ai_persona);
    json_builder_end_object(builder);
    json_builder_end_array(builder);
    json_builder_end_object(builder);
    
    // User Persona
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "role");
    json_builder_add_string_value(builder, "user");
    json_builder_set_member_name(builder, "parts");
    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "text");
    json_builder_add_string_value(builder, config->user_persona);
    json_builder_end_object(builder);
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    // Chat History
    for (guint i = 0; i < conversation->history->len; i++) {
        ChatMessage *msg = g_ptr_array_index(conversation->history, i);
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "role");
        json_builder_add_string_value(builder, msg->role);
        json_builder_set_member_name(builder, "parts");
        json_builder_begin_array(builder);
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "text");
        json_builder_add_string_value(builder, msg->content);
        json_builder_end_object(builder);
        json_builder_end_array(builder);
        json_builder_end_object(builder);
    }

    // Current User Input
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "role");
    json_builder_add_string_value(builder, "user");
    json_builder_set_member_name(builder, "parts");
    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "text");
    json_builder_add_string_value(builder, current_input);
    json_builder_end_object(builder);
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    json_builder_end_array(builder);
    json_builder_end_object(builder);

    JsonGenerator *gen = json_generator_new();
    JsonNode *root = json_builder_get_root(builder);
    
    // 1. Tetapkan 'root' ke generator
    json_generator_set_root(gen, root);
    // 2. Ubah generator menjadi data (string). Argumen kedua adalah untuk panjang, bisa NULL.
    gchar *json_string = json_generator_to_data(gen, NULL);

    json_node_free(root);
    g_object_unref(gen);
    g_object_unref(builder);

    return json_string;
}