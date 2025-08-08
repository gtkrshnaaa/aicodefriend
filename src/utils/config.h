#ifndef AICODEFRIEND_CONFIG_H
#define AICODEFRIEND_CONFIG_H

#include <glib.h>

typedef struct {
    gchar *api_key;
    gchar *system_context;
    gchar *ai_persona;
    gchar *user_persona;
} ConfigData;

ConfigData* config_load(void);
void config_save(ConfigData *config);
void config_free(ConfigData *config);

#endif //AICODEFRIEND_CONFIG_H