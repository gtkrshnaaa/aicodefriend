#include "settings.h"

// Callback for text view changes (for persona)
static void on_persona_text_changed(GtkTextBuffer *buffer, gpointer user_data) {
    gchar **target_string_ptr = (gchar**)user_data;
    if (!target_string_ptr) {
        g_printerr("ERROR: Invalid target_string_ptr in on_persona_text_changed\n");
        return;
    }
    
    g_print("DEBUG: Persona text changed\n");

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    g_autofree gchar *new_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    g_free(*target_string_ptr);
    *target_string_ptr = g_strdup(new_text);
}

// Callback for API key entry changes
static void on_api_key_changed(GtkEntry *entry, gpointer user_data) {
    ConfigData *config = (ConfigData*)user_data;
    if (!config) {
        g_printerr("ERROR: Invalid config in on_api_key_changed\n");
        return;
    }

    g_print("DEBUG: API key changed\n");

    const gchar *new_text = gtk_entry_get_text(entry);
    g_free(config->api_key);
    config->api_key = g_strdup(new_text);
}

// Helper to create expander row for each persona
static GtkWidget* create_persona_row(const gchar *title, const gchar *subtitle, gchar **target_string) {
    if (!title || !subtitle || !target_string) {
        g_printerr("ERROR: Invalid parameters in create_persona_row\n");
        return NULL;
    }

    g_print("DEBUG: Creating persona row: %s\n", title);

    GtkWidget *expander = gtk_expander_new(title);
    if (!expander) {
        g_printerr("ERROR: Failed to create expander\n");
        return NULL;
    }
    gtk_expander_set_label(GTK_EXPANDER(expander), title);
    gtk_expander_set_expanded(GTK_EXPANDER(expander), FALSE);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    if (!vbox) {
        g_printerr("ERROR: Failed to create box\n");
        g_object_unref(expander);
        return NULL;
    }
    GtkWidget *label = gtk_label_new(subtitle);
    if (!label) {
        g_printerr("ERROR: Failed to create label\n");
        g_object_unref(expander);
        g_object_unref(vbox);
        return NULL;
    }
    gtk_widget_set_margin_start(label, 12);
    gtk_widget_set_margin_end(label, 12);
    gtk_widget_set_margin_top(label, 6);
    gtk_widget_set_margin_bottom(label, 6);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    if (!scrolled_window) {
        g_printerr("ERROR: Failed to create scrolled window\n");
        g_object_unref(expander);
        g_object_unref(vbox);
        g_object_unref(label);
        return NULL;
    }
    gtk_widget_set_size_request(scrolled_window, -1, 200);

    GtkWidget *text_view = gtk_text_view_new();
    if (!text_view) {
        g_printerr("ERROR: Failed to create text view\n");
        g_object_unref(expander);
        g_object_unref(vbox);
        g_object_unref(label);
        g_object_unref(scrolled_window);
        return NULL;
    }
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    if (!buffer) {
        g_printerr("ERROR: Failed to get text buffer\n");
        g_object_unref(expander);
        g_object_unref(vbox);
        g_object_unref(label);
        g_object_unref(scrolled_window);
        g_object_unref(text_view);
        return NULL;
    }
    gtk_text_buffer_set_text(buffer, *target_string ? *target_string : "", -1);
    
    g_signal_connect(buffer, "changed", G_CALLBACK(on_persona_text_changed), target_string);
    
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(expander), vbox);

    return expander;
}

GtkWidget* settings_dialog_new(GtkWindow *parent, ConfigData *config) {
    if (!parent || !config) {
        g_printerr("ERROR: Invalid parent or config in settings_dialog_new\n");
        return NULL;
    }

    g_print("DEBUG: Creating settings dialog\n");

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Settings", parent, GTK_DIALOG_MODAL,
                                                   "_OK", GTK_RESPONSE_OK,
                                                   "_Cancel", GTK_RESPONSE_CANCEL,
                                                   NULL);
    if (!dialog) {
        g_printerr("ERROR: Failed to create dialog\n");
        return NULL;
    }
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    if (!content_area) {
        g_printerr("ERROR: Failed to get content area\n");
        g_object_unref(dialog);
        return NULL;
    }
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    if (!vbox) {
        g_printerr("ERROR: Failed to create vbox\n");
        g_object_unref(dialog);
        return NULL;
    }
    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    // API Key Group
    GtkWidget *group_api = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    if (!group_api) {
        g_printerr("ERROR: Failed to create group_api\n");
        g_object_unref(dialog);
        g_object_unref(vbox);
        return NULL;
    }
    gtk_widget_set_margin_start(group_api, 12);
    gtk_widget_set_margin_end(group_api, 12);
    gtk_widget_set_margin_top(group_api, 12);
    gtk_widget_set_margin_bottom(group_api, 12);
    GtkWidget *label_api = gtk_label_new("<b>Koneksi</b>");
    if (!label_api) {
        g_printerr("ERROR: Failed to create label_api\n");
        g_object_unref(dialog);
        g_object_unref(vbox);
        g_object_unref(group_api);
        return NULL;
    }
    gtk_label_set_use_markup(GTK_LABEL(label_api), TRUE);
    gtk_box_pack_start(GTK_BOX(group_api), label_api, FALSE, FALSE, 0);

    GtkWidget *row_api_key = gtk_entry_new();
    if (!row_api_key) {
        g_printerr("ERROR: Failed to create API key entry\n");
        g_object_unref(dialog);
        g_object_unref(vbox);
        g_object_unref(group_api);
        g_object_unref(label_api);
        return NULL;
    }
    gtk_entry_set_placeholder_text(GTK_ENTRY(row_api_key), "Gemini API Key");
    gtk_entry_set_text(GTK_ENTRY(row_api_key), config->api_key ? config->api_key : "");
    g_signal_connect(row_api_key, "changed", G_CALLBACK(on_api_key_changed), config);
    gtk_box_pack_start(GTK_BOX(group_api), row_api_key, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), group_api, FALSE, FALSE, 0);

    // Persona Group
    GtkWidget *group_persona = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    if (!group_persona) {
        g_printerr("ERROR: Failed to create group_persona\n");
        g_object_unref(dialog);
        g_object_unref(vbox);
        g_object_unref(group_api);
        g_object_unref(label_api);
        g_object_unref(row_api_key);
        return NULL;
    }
    gtk_widget_set_margin_start(group_persona, 12);
    gtk_widget_set_margin_end(group_persona, 12);
    gtk_widget_set_margin_top(group_persona, 12);
    gtk_widget_set_margin_bottom(group_persona, 12);
    GtkWidget *label_persona = gtk_label_new("<b>Personalisasi</b>");
    if (!label_persona) {
        g_printerr("ERROR: Failed to create label_persona\n");
        g_object_unref(dialog);
        g_object_unref(vbox);
        g_object_unref(group_api);
        g_object_unref(label_api);
        g_object_unref(row_api_key);
        g_object_unref(group_persona);
        return NULL;
    }
    gtk_label_set_use_markup(GTK_LABEL(label_persona), TRUE);
    gtk_box_pack_start(GTK_BOX(group_persona), label_persona, FALSE, FALSE, 0);
    
    GtkWidget *system_context_row = create_persona_row("System Context", "Aturan dasar untuk AI.", &config->system_context);
    if (system_context_row) {
        gtk_box_pack_start(GTK_BOX(group_persona), system_context_row, FALSE, FALSE, 0);
    } else {
        g_printerr("ERROR: Failed to create system context row\n");
    }

    GtkWidget *ai_persona_row = create_persona_row("AI Persona", "Kepribadian dan gaya bicara AI.", &config->ai_persona);
    if (ai_persona_row) {
        gtk_box_pack_start(GTK_BOX(group_persona), ai_persona_row, FALSE, FALSE, 0);
    } else {
        g_printerr("ERROR: Failed to create AI persona row\n");
    }

    GtkWidget *user_persona_row = create_persona_row("User Persona", "Bagaimana AI melihat Anda.", &config->user_persona);
    if (user_persona_row) {
        gtk_box_pack_start(GTK_BOX(group_persona), user_persona_row, FALSE, FALSE, 0);
    } else {
        g_printerr("ERROR: Failed to create user persona row\n");
    }
    
    gtk_box_pack_start(GTK_BOX(vbox), group_persona, TRUE, TRUE, 0);

    g_signal_connect_swapped(dialog, "response", G_CALLBACK(config_save), config);
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);

    return dialog;
}