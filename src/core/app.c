#include "app.h"
#include "../ui/window.h"
#include <gtk/gtk.h>

static void app_activate(GApplication *application) {
    AICodeFriendApp *self = g_object_get_data(G_OBJECT(application), "app-data");
    
    // Jika belum ada window, buat
    if (!self->main_window) {
        self->main_window = window_new(self);
        gtk_window_set_application(GTK_WINDOW(self->main_window), GTK_APPLICATION(application));
    }
    
    gtk_window_present(GTK_WINDOW(self->main_window));
}

static void app_startup(GApplication *application) {
    G_APPLICATION_CLASS(g_type_class_peek(ADW_TYPE_APPLICATION))->startup(application);

    AICodeFriendApp *self = g_object_get_data(G_OBJECT(application), "app-data");
    
    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    // Menggunakan path yang lebih aman jika style.css tidak ada
    GFile *css_file = g_file_new_for_path("style.css");
    gtk_css_provider_load_from_file(provider, css_file);
    g_object_unref(css_file);

    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Load config dan conversation
    self->config = config_load();
    self->conversation = conversation_new();
}

AICodeFriendApp *app_new(void) {
    AICodeFriendApp *self = g_new0(AICodeFriendApp, 1);
    
    // Menggunakan Application ID yang valid (format reverse-DNS)
    self->parent = adw_application_new("com.gtkrshnaaa.aicodefriend", G_APPLICATION_DEFAULT_FLAGS);
    
    // Simpan pointer 'self' ke dalam objek GApplication agar bisa diakses di callback
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