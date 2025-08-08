#include "string_utils.h"

gchar* string_utils_trim(const gchar *input) {
    if (!input) return NULL;
    return g_strstrip(g_strdup(input));
}

gchar* string_utils_replace(const gchar *input, const gchar *search, const gchar *replace) {
    if (!input || !search || !replace) return NULL;
    return g_strreplace(input, search, replace, 0);
}