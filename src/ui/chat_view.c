#include "chat_view.h"
#include <gtksourceview/gtksource.h>

void chat_view_add_message(ChatView *chat_view, const gchar *text, ChatMessageType type) {
    if (!chat_view || !text) {
        g_printerr("ERROR: Invalid chat_view or text in chat_view_add_message\n");
        return;
    }

    g_print("DEBUG: Adding message: %s\n", text);

    GtkWidget *bubble_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    if (!bubble_box) {
        g_printerr("ERROR: Failed to create bubble_box\n");
        return;
    }

    GtkWidget *label = gtk_label_new(NULL);
    if (!label) {
        g_printerr("ERROR: Failed to create label\n");
        g_object_unref(bubble_box);
        return;
    }
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    
    gtk_widget_add_css_class(label, "bubble");

    if (type == CHAT_MESSAGE_USER) {
        gtk_widget_set_halign(bubble_box, GTK_ALIGN_END);
        gtk_widget_add_css_class(label, "user-bubble");
    } else {
        gtk_widget_set_halign(bubble_box, GTK_ALIGN_START);
        gtk_widget_add_css_class(label, "ai-bubble");
    }

    gtk_box_append(GTK_BOX(bubble_box), label);
    gtk_box_append(GTK_BOX(chat_view->message_box), bubble_box);
}

void chat_view_clear(ChatView *chat_view) {
    if (!chat_view) {
        g_printerr("ERROR: Invalid chat_view in chat_view_clear\n");
        return;
    }

    g_print("DEBUG: Clearing chat view\n");

    GtkWidget *child = gtk_widget_get_first_child(chat_view->message_box);
    while (child != NULL) {
        GtkWidget *next_child = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(chat_view->message_box), child);
        child = next_child;
    }
}

ChatView* chat_view_new(void) {
    g_print("DEBUG: Creating new chat view\n");

    ChatView *cv = g_new(ChatView, 1);
    if (!cv) {
        g_printerr("ERROR: Failed to allocate ChatView\n");
        return NULL;
    }

    cv->scrolled_window = gtk_scrolled_window_new();
    if (!cv->scrolled_window) {
        g_printerr("ERROR: Failed to create scrolled_window\n");
        g_free(cv);
        return NULL;
    }
    gtk_widget_set_vexpand(cv->scrolled_window, TRUE);

    cv->message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    if (!cv->message_box) {
        g_printerr("ERROR: Failed to create message_box\n");
        g_object_unref(cv->scrolled_window);
        g_free(cv);
        return NULL;
    }
    gtk_widget_set_margin_start(cv->message_box, 12);
    gtk_widget_set_margin_end(cv->message_box, 12);
    gtk_widget_set_margin_top(cv->message_box, 12);
    gtk_widget_set_margin_bottom(cv->message_box, 12);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(cv->scrolled_window), cv->message_box);

    chat_view_add_message(cv, "Hii, lets make some code.", CHAT_MESSAGE_AI);
    
    return cv;
}

void chat_view_free(ChatView *cv) {
    if (!cv) {
        g_printerr("ERROR: Invalid chat_view in chat_view_free\n");
        return;
    }

    g_print("DEBUG: Freeing chat view\n");

    g_free(cv);
}