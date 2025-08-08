#ifndef AICODEFRIEND_GEMINI_CLIENT_H
#define AICODEFRIEND_GEMINI_CLIENT_H

#include <glib.h>

// Deklarasi fungsi untuk mengirim prompt ke API.
// Ini akan dijalankan di worker thread.
gchar* gemini_client_send_prompt(const gchar *api_key, const gchar *json_payload);

#endif //AICODEFRIEND_GEMINI_CLIENT_H