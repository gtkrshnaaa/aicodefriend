#ifndef AICODEFRIEND_PROMPT_H
#define AICODEFRIEND_PROMPT_H

#include <glib.h>
#include "../utils/config.h"
#include "../core/conversation.h"

gchar* prompt_build_json(ConfigData *config, Conversation *conversation, const gchar *current_input);

#endif //AICODEFRIEND_PROMPT_H