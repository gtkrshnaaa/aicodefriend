#include "window.h"
#include "settings.h"
#include "../api/gemini_client.h"
#include "../api/prompt.h"
#include <json-glib/json-glib.h>

#include "../core/app.h"

// Struct untuk membawa data ke worker thread
typedef struct {
    AICodeFriendApp *app;
    gchar *user_input;
} ThreadData;

// Deklarasi fungsi-fungsi statis yang akan digunakan di file ini
static void send_request_finish(GObject *source_object, GAsyncResult *res, gpointer user_data);
static void send_request_in_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable);
static void on_send_button_clicked(GtkButton *button, gpointer user_data);
static void on_settings_button_clicked(GtkButton *button, gpointer user_data);
static void on_new_chat_clicked(GtkButton *button, gpointer user_data);

// Fungsi untuk membebaskan ThreadData
static void thread_data_free(ThreadData *data) {
    if (data) {
        g_free(data->user_input);
        g_free(data);
    }
}

// Fungsi ini akan dijalankan di worker thread
static void send_request_in_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    ThreadData *data = (ThreadData*)task_data;
    AICodeFriendApp *app = data->app;

    g_print("DEBUG: Sending request in thread with input: %s\n", data->user_input);

    gchar *json_payload = prompt_build_json(app->config, app->conversation, data->user_input);
    if (!json_payload) {
        g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Gagal membangun JSON payload.");
        return;
    }

    gchar *api_response_body = gemini_client_send_prompt(app->config->api_key, json_payload);
    g_free(json_payload);
    
    if (!api_response_body) {
        g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Gagal menghubungi API. Periksa koneksi atau API Key Anda.");
        return;
    }

    g_print("DEBUG: Received API response: %s\n", api_response_body);

    g_autoptr(JsonParser) parser = json_parser_new();
    if (json_parser_load_from_data(parser, api_response_body, -1, NULL)) {
        g_autoptr(JsonNode) root = json_parser_get_root(parser);
        JsonObject *obj = json_node_get_object(root);
        
        if (json_object_has_member(obj, "error")) {
            JsonObject *error_obj = json_object_get_object_member(obj, "error");
            const gchar *error_msg = json_object_get_string_member_with_default(error_obj, "message", "Terjadi eror tidak diketahui dari API.");
            g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Eror API: %s", error_msg);
        } else {
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
        }
    } else {
        g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_FAILED, "Gagal mem-parsing respons AI.");
    }
    g_free(api_response_body);
}

// Callback ini akan dieksekusi di Main Thread setelah worker thread selesai
static void send_request_finish(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;
    g_autoptr(GError) error = NULL;
    gchar *ai_response = g_task_propagate_pointer(G_TASK(res), &error);

    g_print("DEBUG: Send request finished with response: %s\n", ai_response ? ai_response : "NULL");

    if (error) {
        chat_view_add_message(app->chat_view, error->message, CHAT_MESSAGE_AI);
    } else {
        chat_view_add_message(app->chat_view, ai_response, CHAT_MESSAGE_AI);
        conversation_add_message(app->conversation, "model", ai_response);
    }
    gtk_widget_set_sensitive(app->send_button, TRUE);
}

// Callback untuk tombol kirim
static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;
    if (!app || !app->text_entry_buffer) {
        g_printerr("ERROR: Invalid app or text_entry_buffer\n");
        return;
    }

    g_print("DEBUG: Send button clicked\n");

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(app->text_entry_buffer, &start, &end);
    gchar *user_input_raw = gtk_text_buffer_get_text(app->text_entry_buffer, &start, &end, FALSE);
    if (!user_input_raw) {
        g_printerr("ERROR: Failed to get text from buffer\n");
        return;
    }

    g_autofree gchar *user_input = g_strstrip(user_input_raw);
    if (g_str_equal(user_input, "")) {
        g_free(user_input_raw);
        g_print("DEBUG: Empty input, ignoring\n");
        return;
    }

    gtk_widget_set_sensitive(app->send_button, FALSE);
    chat_view_add_message(app->chat_view, user_input, CHAT_MESSAGE_USER);
    conversation_add_message(app->conversation, "user", user_input);
    
    GtkWidget *history_row = gtk_label_new(user_input);
    gtk_label_set_xalign(GTK_LABEL(history_row), 0.0);
    gtk_list_box_append(GTK_LIST_BOX(app->history_list_box), history_row);

    gtk_text_buffer_set_text(app->text_entry_buffer, "", -1);

    ThreadData *thread_data = g_new0(ThreadData, 1);
    if (!thread_data) {
        g_printerr("ERROR: Failed to allocate ThreadData\n");
        gtk_widget_set_sensitive(app->send_button, TRUE);
        g_free(user_input_raw);
        return;
    }

    thread_data->app = app;
    thread_data->user_input = g_strdup(user_input);
    g_free(user_input_raw);

    GTask *task = g_task_new(button, NULL, send_request_finish, app);
    g_task_set_task_data(task, thread_data, (GDestroyNotify)thread_data_free);
    g_task_run_in_thread(task, send_request_in_thread);
    g_object_unref(task);
}

// Callback untuk tombol settings
static void on_settings_button_clicked(GtkButton *button, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;
    if (!app || !app->main_window) {
        g_printerr("ERROR: Invalid app or main_window\n");
        return;
    }

    g_print("DEBUG: Settings button clicked\n");

    AdwDialog *settings_dialog = settings_dialog_new(GTK_WINDOW(app->main_window), app->config);
    if (settings_dialog) {
        g_print("DEBUG: Presenting settings dialog\n");
        adw_dialog_present(settings_dialog, app->main_window);
    } else {
        g_printerr("ERROR: Failed to create settings dialog\n");
    }
}

// Callback untuk tombol new chat
static void on_new_chat_clicked(GtkButton *button, gpointer user_data) {
    AICodeFriendApp *app = (AICodeFriendApp*)user_data;
    if (!app) {
        g_printerr("ERROR: Invalid app\n");
        return;
    }

    g_print("DEBUG: New chat button clicked\n");

    conversation_free(app->conversation);
    app->conversation = conversation_new();
    
    chat_view_clear(app->chat_view);
    chat_view_add_message(app->chat_view, "Percakapan baru dimulai.", CHAT_MESSAGE_AI);
}

// Fungsi utama pembuat jendela
GtkWidget *window_new(AICodeFriendApp *app) {
    if (!app) {
        g_printerr("ERROR: Invalid app\n");
        return NULL;
    }

    g_print("DEBUG: Creating new window\n");

    GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app->parent));
    gtk_window_set_title(GTK_WINDOW(window), "AI CodeFriend");
    gtk_window_set_default_size(GTK_WINDOW(window), 1100, 750);

    GtkWidget *root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), root_box);

    // --- Header Bar ---
    GtkWidget *header = adw_header_bar_new();
    gtk_box_append(GTK_BOX(root_box), header);
    
    GtkWidget *toggle_button = gtk_toggle_button_new();
    gtk_button_set_icon_name(GTK_BUTTON(toggle_button), "view-sidebar-symbolic");
    adw_header_bar_pack_start(ADW_HEADER_BAR(header), toggle_button);

    GtkWidget *new_chat_button = gtk_button_new_with_label("New Chat");
    adw_header_bar_pack_start(ADW_HEADER_BAR(header), new_chat_button);
    g_signal_connect(new_chat_button, "clicked", G_CALLBACK(on_new_chat_clicked), app);
    
    GtkWidget *settings_button = gtk_button_new_with_label("Settings");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header), settings_button);
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_settings_button_clicked), app);
    
    // --- Layout Utama dengan AdwOverlaySplitView ---
    GtkWidget *split_view = adw_overlay_split_view_new();
    gtk_box_append(GTK_BOX(root_box), split_view);

    g_object_bind_property(toggle_button, "active", split_view, "show-sidebar", G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);

    // -- Sidebar --
    GtkWidget *sidebar_scrolled = gtk_scrolled_window_new();
    adw_overlay_split_view_set_sidebar(ADW_OVERLAY_SPLIT_VIEW(split_view), sidebar_scrolled);
    app->history_list_box = gtk_list_box_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sidebar_scrolled), app->history_list_box);

    // -- Content (Chat Area + Input) --
    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    adw_overlay_split_view_set_content(ADW_OVERLAY_SPLIT_VIEW(split_view), content_box);
    
    app->chat_view = chat_view_new();
    gtk_box_append(GTK_BOX(content_box), app->chat_view->scrolled_window);

    // Input Bar
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
    gtk_box_append(GTK_BOX(content_box), input_bar);
    
    g_signal_connect(app->send_button, "clicked", G_CALLBACK(on_send_button_clicked), app);

    return window;
}