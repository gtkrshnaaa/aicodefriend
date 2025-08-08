#include "config.h"
#include <json-glib/json-glib.h>
#include <glib/gstdio.h>

static const gchar* SETTINGS_FILE = "settings.json";

static gchar* get_config_path(void) {
    return g_build_filename(g_get_user_config_dir(), "aicodefriend", SETTINGS_FILE, NULL);
}

static void ensure_config_dir_exists(void) {
    gchar *dir_path = g_build_filename(g_get_user_config_dir(), "aicodefriend", NULL);
    g_mkdir_with_parents(dir_path, 0755);
    g_free(dir_path);
}

ConfigData* config_load(void) {
    ensure_config_dir_exists();
    gchar *path = get_config_path();
    ConfigData *config = g_new0(ConfigData, 1);
    
    JsonParser *parser = json_parser_new();
    if (!json_parser_load_from_file(parser, path, NULL)) {
        // File not found or invalid, create default values
        g_print("Settings file not found or invalid. Creating with default values.\n");
        config->api_key = g_strdup("");
        config->system_context = g_strdup("This is an AI Coding Companion. AI model core task is to assist the user by providing structured responses. All normal text must be formatted using Pango Markup. All code blocks must be enclosed within the custom tags <code-block lang=\"...\">...</code-block>. It is strictly forbidden to use triple backtick markdown (```).");
        config->ai_persona = g_strdup("Ai is an assistant with a calm, friendly, and wise personality, acting as a thoughtful companion.");
        config->user_persona = g_strdup("The user is a Software Engineer experienced in C, Java, Go, PHP, JS, and Python. The user likes writing clean and tidy code.");
        config_save(config);
    } else {
        JsonNode *root = json_parser_get_root(parser);
        JsonObject *object = json_node_get_object(root);
        config->api_key = g_strdup(json_object_get_string_member_with_default(object, "api_key", ""));
        config->system_context = g_strdup(json_object_get_string_member_with_default(object, "system_context", ""));
        config->ai_persona = g_strdup(json_object_get_string_member_with_default(object, "ai_persona", ""));
        config->user_persona = g_strdup(json_object_get_string_member_with_default(object, "user_persona", ""));
    }

    g_object_unref(parser);
    g_free(path);
    return config;
}

void config_save(ConfigData *config) {
    gchar *path = get_config_path();
    JsonBuilder *builder = json_builder_new();
    
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "api_key");
    json_builder_add_string_value(builder, config->api_key);
    json_builder_set_member_name(builder, "system_context");
    json_builder_add_string_value(builder, config->system_context);
    json_builder_set_member_name(builder, "ai_persona");
    json_builder_add_string_value(builder, config->ai_persona);
    json_builder_set_member_name(builder, "user_persona");
    json_builder_add_string_value(builder, config->user_persona);
    json_builder_end_object(builder);
    
    JsonGenerator *gen = json_generator_new();
    json_generator_set_pretty(gen, TRUE);
    JsonNode *root = json_builder_get_root(builder);
    json_generator_to_file(gen, root, path, NULL);
    
    json_node_free(root);
    g_object_unref(gen);
    g_object_unref(builder);
    g_free(path);
}

void config_free(ConfigData *config) {
    g_free(config->api_key);
    g_free(config->system_context);
    g_free(config->ai_persona);
    g_free(config->user_persona);
    g_free(config);
}