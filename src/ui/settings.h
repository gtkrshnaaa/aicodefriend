#ifndef AICODEFRIEND_SETTINGS_H
#define AICODEFRIEND_SETTINGS_H

#include <gtk/gtk.h>
#include "../utils/config.h"

GtkWidget* settings_dialog_new(GtkWindow *parent, ConfigData *config);

#endif //AICODEFRIEND_SETTINGS_H