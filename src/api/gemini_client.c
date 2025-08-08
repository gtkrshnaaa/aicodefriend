#include "gemini_client.h"
#include <libsoup/soup.h>

// URL endpoint untuk Gemini API
static const gchar *GEMINI_API_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent";

gchar* gemini_client_send_prompt(const gchar *api_key, const gchar *json_payload) {
    // Membuat session untuk koneksi HTTP
    SoupSession *session = soup_session_new();
    gchar *url = g_strdup_printf("%s?key=%s", GEMINI_API_URL, api_key);

    // Membuat pesan request baru dengan method POST ke URL yang sudah ada API key-nya
    SoupMessage *msg = soup_message_new(SOUP_METHOD_POST, url);
    if (!msg) {
        g_printerr("Failed to create SoupMessage.\n");
        g_free(url);
        g_object_unref(session);
        return NULL;
    }

    // Menetapkan body dari request dengan payload JSON kita
    soup_message_set_request_body_from_bytes(msg,
                                             "application/json",
                                             g_bytes_new(json_payload, strlen(json_payload)));

    // Mengirim pesan secara sinkron (karena kita sudah di dalam worker thread)
    GBytes *response_bytes = soup_session_send_and_read(session, msg, NULL, NULL);

    if (!response_bytes) {
        guint status = soup_message_get_status(msg);
        g_printerr("Failed to send request. HTTP Status: %u\n", status);
        g_free(url);
        g_object_unref(msg);
        g_object_unref(session);
        return NULL;
    }

    // Mengambil data dari response
    gsize size;
    const gchar *data = g_bytes_get_data(response_bytes, &size);
    gchar *response_body = g_strndup(data, size);

    // Membersihkan memori
    g_bytes_unref(response_bytes);
    g_free(url);
    g_object_unref(msg);
    g_object_unref(session);

    return response_body;
}