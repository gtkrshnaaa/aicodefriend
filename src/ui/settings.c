#include "settings.h"

// Callback untuk menyimpan perubahan pada text view (untuk persona)
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

// Helper untuk membuat baris expander untuk setiap persona
static GtkWidget* create_persona_row(const gchar *title, const gchar *subtitle, gchar **target_string) {
    if (!title || !subtitle || !target_string) {
        g_printerr("ERROR: Invalid parameters in create_persona_row\n");
        return NULL;
    }

    g_print("DEBUG: Creating persona row: %s\n", title);

    GtkWidget *row = adw_expander_row_new();
    if (!row) {
        g_printerr("ERROR: Failed to create expander row\n");
        return NULL;
    }
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), title);
    adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(row), subtitle);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(scrolled_window, -1, 200);

    GtkWidget *text_view = gtk_text_view_new();
    if (!text_view) {
        g_printerr("ERROR: Failed to create text view\n");
        g_object_unref(row);
        return NULL;
    }
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, *target_string ? *target_string : "", -1);
    
    g_signal_connect(buffer, "changed", G_CALLBACK(on_persona_text_changed), target_string);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);
    adw_expander_row_add_row(ADW_EXPANDER_ROW(row), scrolled_window);

    return row;
}

AdwDialog* settings_dialog_new(GtkWindow *parent, ConfigData *config) {
    if (!parent || !config) {
        g_printerr("ERROR: Invalid parent or config in settings_dialog_new\n");
        return NULL;
    }

    g_print("DEBUG: Creating settings dialog\n");

    AdwDialog *dialog = adw_preferences_dialog_new();
    if (!dialog) {
        g_printerr("ERROR: Failed to create AdwPreferencesDialog\n");
        return NULL;
    }

    GtkWidget *page = adw_preferences_page_new();
    if (!page) {
        g_printerr("ERROR: Failed to create AdwPreferencesPage\n");
        g_object_unref(dialog);
        return NULL;
    }
    adw_preferences_dialog_add(ADW_PREFERENCES_DIALOG(dialog), ADW_PREFERENCES_PAGE(page));

    GtkWidget *group_api = adw_preferences_group_new();
    if (!group_api) {
        g_printerr("ERROR: Failed to create API preferences group\n");
        g_object_unref(dialog);
        return NULL;
    }
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(group_api), "Koneksi");
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(group_api));

    GtkWidget *row_api_key = adw_entry_row_new();
    if (!row_api_key) {
        g_printerr("ERROR: Failed to create API key entry row\n");
        g_object_unref(dialog);
        return NULL;
    }
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_api_key), "Gemini API Key");
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(group_api), row_api_key);
    g_object_bind_property(config, "api_key", row_api_key, "text", G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);

    GtkWidget *group_persona = adw_preferences_group_new();
    if (!group_persona) {
        g_printerr("ERROR: Failed to create persona preferences group\n");
        g_object_unref(dialog);
        return NULL;
    }
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(group_persona), "Personalisasi");
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(group_persona));
    
    GtkWidget *system_context_row = create_persona_row("System Context", "Aturan dasar untuk AI.", &config->system_context);
    if (system_context_row) {
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(group_persona), system_context_row);
    } else {
        g_printerr("ERROR: Failed to create system context row\n");
    }

    GtkWidget *ai_persona_row = create_persona_row("AI Persona", "Kepribadian dan gaya bicara AI.", &config->ai_persona);
    if (ai_persona_row) {
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(group_persona), ai_persona_row);
    } else {
        g_printerr("ERROR: Failed to create AI persona row\n");
    }

    GtkWidget *user_persona_row = create_persona_row("User Persona", "Bagaimana AI melihat Anda.", &config->user_persona);
    if (user_persona_row) {
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(group_persona), user_persona_row);
    } else {
        g_printerr("ERROR: Failed to create user persona row\n");
    }
    
    g_signal_connect_swapped(dialog, "close-request", G_CALLBACK(config_save), config);

    return dialog;
}