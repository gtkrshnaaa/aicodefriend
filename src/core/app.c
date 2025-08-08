#include "app.h"
#include "../ui/window.h"
#include <gtk/gtk.h>

static void app_activate(GApplication *application) {
    AICodeFriendApp *self = g_object_get_data(G_OBJECT(application), "app-data");
    
    // Create window if not exists
    if (!self->main_window) {
        self->main_window = window_new(self);
        gtk_window_set_application(GTK_WINDOW(self->main_window), GTK_APPLICATION(application));
    }
    
    gtk_window_present(GTK_WINDOW(self->main_window));
}

static void app_startup(GApplication *application) {
    G_APPLICATION_CLASS(g_type_class_peek(GTK_TYPE_APPLICATION))->startup(application);

    AICodeFriendApp *self = g_object_get_data(G_OBJECT(application), "app-data");
    
    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path("assets/styles/style.css");
    GError *error = NULL;
    
    if (!gtk_css_provider_load_from_file(provider, css_file, &error)) {
        g_printerr("ERROR: Failed to load CSS file: %s\n", error ? error->message : "Unknown error");
        g_clear_error(&error);
    }
    
    g_object_unref(css_file);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Load config and conversation
    self->config = config_load();
    self->conversation = conversation_new();
}

AICodeFriendApp *app_new(void) {
    AICodeFriendApp *self = g_new0(AICodeFriendApp, 1);
    
    // Use valid Application ID
    self->parent = gtk_application_new("com.gtkrshnaaa.aicodefriend", G_APPLICATION_DEFAULT_FLAGS);
    
    // Store self in GApplication for callbacks
    g_object_set_data(G_OBJECT(self->parent), "app-data", self);
    
    g_signal_connect(self->parent, "startup", G_CALLBACK(app_startup), NULL);
    g_signal_connect(self->parent, "activate", G_CALLBACK(app_activate), NULL);
    
    return self;
}

void app_free(AICodeFriendApp *app) {
    config_free(app->config);
    conversation_free(app->conversation);
    g_object_unref(app->parent);
    g_free(app);
}