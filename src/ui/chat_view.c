#include "chat_view.h"
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include "../core/app.h"

struct _ChatView {
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
};

ChatView* chat_view_new() {
    ChatView *view = g_new(ChatView, 1);
    
    view->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(view->scrolled_window, TRUE);
    gtk_widget_set_vexpand(view->scrolled_window, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(view->scrolled_window), 
                                  GTK_POLICY_AUTOMATIC, 
                                  GTK_POLICY_AUTOMATIC);
    
    view->text_view = gtk_text_view_new();
    view->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view->text_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view->text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view->text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view->text_view), 12);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view->text_view), 12);
    
    gtk_container_add(GTK_CONTAINER(view->scrolled_window), view->text_view);
    
    g_print("DEBUG: Creating new chat view\n");
    
    // Apply CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Initialize tags for styling
    gtk_text_buffer_create_tag(view->buffer, "user-bubble", 
                              "paragraph-spacing", 12,
                              "wrap-mode", GTK_WRAP_WORD_CHAR,
                              "background", "#729fcf",
                              "foreground", "white",
                              "left-margin", 24,
                              "right-margin", 12,
                              NULL);
    gtk_text_buffer_create_tag(view->buffer, "ai-bubble",
                              "paragraph-spacing", 12,
                              "wrap-mode", GTK_WRAP_WORD_CHAR,
                              "background", "#eeeeec",
                              "foreground", "#2e3436",
                              "left-margin", 12,
                              "right-margin", 24,
                              NULL);
    gtk_text_buffer_create_tag(view->buffer, "code-block",
                              "family", "Monospace",
                              "background", "#2e3436",
                              "foreground", "#ffffff",
                              "left-margin", 24,
                              "right-margin", 24,
                              "indent", -20,
                              NULL);

    return view;
}

void chat_view_add_message(ChatView *view, const gchar *message, ChatMessageType type) {
    if (!view || !message) {
        g_printerr("ERROR: Invalid view or message\n");
        return;
    }

    g_print("DEBUG: Adding message: %s\n", message);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(view->buffer, &end);
    
    // Determine tag based on type
    const gchar *tag_name = (type == CHAT_MESSAGE_USER) ? "user-bubble" : "ai-bubble";

    // Parse for code blocks
    GString *formatted_message = g_string_new(NULL);
    const gchar *ptr = message;
    const gchar *code_start = NULL;
    
    while (*ptr) {
        if (g_str_has_prefix(ptr, "<code-block>")) {
            if (code_start) {
                // End of code block
                g_autofree gchar *code_content = g_strndup(code_start, ptr - code_start);
                g_string_append(formatted_message, code_content);
                code_start = NULL;
            }
            ptr += strlen("<code-block>");
            gtk_text_buffer_insert_with_tags_by_name(view->buffer, &end, "\n", -1, "code-block", NULL);
        } else if (code_start) {
            ptr++;
        } else if (*ptr == '\n') {
            g_string_append_c(formatted_message, *ptr);
            gtk_text_buffer_insert_with_tags_by_name(view->buffer, &end, "\n", -1, tag_name, NULL);
            ptr++;
        } else {
            if (!code_start) {
                g_string_append_c(formatted_message, *ptr);
            }
            ptr++;
        }
    }

    if (code_start) {
        g_autofree gchar *code_content = g_strndup(code_start, ptr - code_start);
        g_string_append(formatted_message, code_content);
    }

    gtk_text_buffer_insert_with_tags_by_name(view->buffer, &end, formatted_message->str, -1, tag_name, NULL);
    gtk_text_buffer_insert(view->buffer, &end, "\n\n", -1);

    // Scroll to bottom
    GtkTextMark *mark = gtk_text_buffer_create_mark(view->buffer, NULL, &end, FALSE);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(view->text_view), mark, 0.0, FALSE, 0.0, 0.0);
    gtk_text_buffer_delete_mark(view->buffer, mark);

    g_string_free(formatted_message, TRUE);
}

void chat_view_clear(ChatView *view) {
    if (!view) {
        g_printerr("ERROR: Invalid view\n");
        return;
    }
    
    gtk_text_buffer_set_text(view->buffer, "", -1);
}