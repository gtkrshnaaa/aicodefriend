#include "settings.h"

// Callback untuk menyimpan perubahan pada text view (untuk persona)
static void on_persona_text_changed(GtkTextBuffer *buffer, gpointer user_data) {
    gchar **target_string_ptr = (gchar**)user_data;
    
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    g_autofree gchar *new_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    g_free(*target_string_ptr);
    *target_string_ptr = g_strdup(new_text);
}

// Helper untuk membuat baris expander untuk setiap persona
static GtkWidget* create_persona_row(const gchar *title, const gchar *subtitle, gchar **target_string) {
    GtkWidget *row = adw_expander_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), title);
    adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(row), subtitle);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(scrolled_window, -1, 200);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, *target_string, -1);
    
    g_signal_connect(buffer, "changed", G_CALLBACK(on_persona_text_changed), target_string);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);
    adw_expander_row_add_row(ADW_EXPANDER_ROW(row), scrolled_window);

    return row;
}


GtkWidget* settings_dialog_new(GtkWindow *parent, ConfigData *config) {
    // --- PERBAIKAN DI SINI ---
    // adw_dialog_new() tidak menerima argumen.
    // Judul dan parent di-set secara terpisah setelah dialog dibuat.
    GtkWidget *dialog = adw_dialog_new();
    gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
    gtk_window_set_title(GTK_WINDOW(dialog), "Pengaturan");
    // --- AKHIR PERBAIKAN ---

    gtk_window_set_default_size(GTK_WINDOW(dialog), 520, 600);

    GtkWidget *page = adw_preferences_page_new();
    adw_dialog_set_child(ADW_DIALOG(dialog), page);
    
    // Grup untuk API Key
    GtkWidget *group_api = adw_preferences_group_new();
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(group_api));
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(group_api), "Koneksi");

    GtkWidget *row_api_key = adw_entry_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_api_key), "Gemini API Key");
    g_object_bind_property(config, "api_key", row_api_key, "text", G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(group_api), row_api_key);

    // Grup untuk Persona
    GtkWidget *group_persona = adw_preferences_group_new();
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(group_persona));
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(group_persona), "Personalisasi");

    gtk_list_box_append(GTK_LIST_BOX(group_persona), create_persona_row("System Context", "Aturan dasar untuk AI.", &config->system_context));
    gtk_list_box_append(GTK_LIST_BOX(group_persona), create_persona_row("AI Persona", "Kepribadian dan gaya bicara AI.", &config->ai_persona));
    gtk_list_box_append(GTK_LIST_BOX(group_persona), create_persona_row("User Persona", "Bagaimana AI melihat Anda.", &config->user_persona));
    
    g_signal_connect_swapped(dialog, "close-request", G_CALLBACK(config_save), config);

    return dialog;
}