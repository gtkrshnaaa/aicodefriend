#include "window.h"
#include "settings.h"
#include "../api/gemini_client.h"
#include "../api/prompt.h"
#include <json-glib/json-glib.h>

// Menyertakan definisi AICodeFriendApp dan ChatMessageType
#include "../core/app.h"
#include "chat_view.h"


// Struct untuk membawa data ke worker thread
typedef struct {
    AICodeFriendApp *app;
    gchar *user_input;
} ThreadData;

static void send_request_finish(GObject *source_object, GAsyncResult *res, gpointer user_data);

// Fungsi ini akan dijalankan di worker thread
static void send_request_in_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    ThreadData *data = (ThreadData*)task_data;
    AICodeFriendApp *app = data->app;

    gchar *json_payload = prompt_build_json(app->config, app->conversation, data->user_input);
    gchar *api_response_body = gemini_client_send_prompt(app->config->api_key, json_payload);
    g_free(json_payload);
    
    if (api_response_body == NULL) {
        g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Gagal menghubungi API. Periksa koneksi atau API Key Anda.");
        return;
    }

    JsonParser *parser = json_parser_new();
    if (json_parser_load_from_data(parser, api_response_body, -1, NULL)) {
        JsonNode *root = json_parser_get_root(parser);
        JsonObject *obj = json_node_get_object(root);
        JsonArray *candidates = json_object_get_array_member(obj, "candidates");
        if (candidates && json_array_get_length(candidates) > 0) {
            JsonObject *first_candidate = json_array_get_object_element(candidates, 0);
            JsonObject *content = json_object_get_object_member(first_candidate, "content");
            JsonArray *parts = json_object_get_array_member(content, "parts");
            JsonObject *first_part = json_array_get_object_element(parts, 0);
            const gchar *ai_text = json_object_get_string_member(first_part, "text");
            
            g_task_return_pointer(task, g_strdup(ai_text), g_free);
        } else {
             g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Respons AI tidak valid atau kosong.");
        }
    } else {
        g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Gagal mem-parsing respons AI.");
    }

    g_free(api_response_body);
    g_object_unref(parser);
}

// Callback ini akan dieksekusi di Main Thread setelah worker thread selesai
static void send_request_finish(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;
    g_autoptr(GError) error = NULL;
    gchar *ai_response = g_task_propagate_pointer(G_TASK(res), &error);

    if (error) {
        chat_view_add_message(app->chat_view, error->message, CHAT_MESSAGE_AI);
    } else {
        chat_view_add_message(app->chat_view, ai_response, CHAT_MESSAGE_AI);
        conversation_add_message(app->conversation, "model", ai_response);
    }
    
    gtk_widget_set_sensitive(app->send_button, TRUE);
}

static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(app->text_entry_buffer, &start, &end);
    gchar *user_input_raw = gtk_text_buffer_get_text(app->text_entry_buffer, &start, &end, FALSE);
    gchar *user_input = g_strstrip(user_input_raw); // Hilangkan spasi di awal/akhir
    
    if (g_str_equal(user_input, "")) {
        g_free(user_input_raw);
        return;
    }

    gtk_widget_set_sensitive(app->send_button, FALSE);
    chat_view_add_message(app->chat_view, user_input, CHAT_MESSAGE_USER);
    conversation_add_message(app->conversation, "user", user_input);
    gtk_text_buffer_set_text(app->text_entry_buffer, "", -1);

    ThreadData *thread_data = g_new(ThreadData, 1);
    thread_data->app = app;
    thread_data->user_input = g_strdup(user_input);

    GTask *task = g_task_new(button, NULL, send_request_finish, app);
    g_task_set_task_data(task, thread_data, (GDestroyNotify)g_free);
    g_task_run_in_thread(task, send_request_in_thread);
    g_object_unref(task);
    g_free(user_input_raw);
}

static void on_settings_button_clicked(GtkButton *button, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;
    GtkWidget *settings_win = settings_dialog_new(GTK_WINDOW(app->main_window), app->config);
    gtk_window_present(GTK_WINDOW(settings_win)); // DIPERBAIKI: Menggunakan fungsi modern
}

// DIPERBAIKI: Menerima AICodeFriendApp*
GtkWidget *window_new(AICodeFriendApp *app) {
    // DIPERBAIKI: Menggunakan app->parent untuk inisialisasi
    GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app->parent));
    gtk_window_set_title(GTK_WINDOW(window), "AI CodeFriend");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 700);

    GtkWidget *header = adw_header_bar_new();
    GtkWidget *settings_button = gtk_button_new_from_icon_name("preferences-system-symbolic");
    gtk_widget_set_tooltip_text(settings_button, "Pengaturan");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header), settings_button);
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_settings_button_clicked), app);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(main_box), header);

    app->chat_view = chat_view_new();
    gtk_box_append(GTK_BOX(main_box), app->chat_view);

    GtkWidget *input_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_start(input_bar, 12);
    gtk_widget_set_margin_end(input_bar, 12);
    gtk_widget_set_margin_top(input_bar, 6);
    gtk_widget_set_margin_bottom(input_bar, 12);

    GtkWidget *scrolled_input = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(scrolled_input, TRUE);
    gtk_scrolled_window_set_max_content_height(GTK_SCROLLED_WINDOW(scrolled_input), 150);
    gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_input), TRUE);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    app->text_entry_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_input), text_view);

    app->send_button = gtk_button_new_from_icon_name("document-send-symbolic");
    gtk_widget_set_valign(app->send_button, GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(input_bar), scrolled_input);
    gtk_box_append(GTK_BOX(input_bar), app->send_button);
    gtk_box_append(GTK_BOX(main_box), input_bar);

    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), main_box);
    
    g_signal_connect(app->send_button, "clicked", G_CALLBACK(on_send_button_clicked), app);

    return window;
}