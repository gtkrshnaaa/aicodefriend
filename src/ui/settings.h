#ifndef AICODEFRIEND_SETTINGS_H
#define AICODEFRIEND_SETTINGS_H

#include <adwaita.h>
#include "../utils/config.h"

// Fungsi untuk membuat dialog settings baru
AdwDialog* settings_dialog_new(GtkWindow *parent, ConfigData *config);

#endif //AICODEFRIEND_SETTINGS_H