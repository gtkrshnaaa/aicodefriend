#include "string_utils.h"

gchar* string_utils_trim(const gchar *input) {
    if (!input) return NULL;
    return g_strstrip(g_strdup(input));
}

gchar* string_utils_replace(const gchar *input, const gchar *search, const gchar *replace) {
    if (!input || !search || !replace) return NULL;

    // Initialize a GString for building the result
    GString *result = g_string_new(NULL);
    const gchar *current = input;
    const gchar *match;

    // Iterate through the input string, looking for matches
    while ((match = g_strstr_len(current, -1, search)) != NULL) {
        // Append the part before the match
        g_string_append_len(result, current, match - current);
        // Append the replacement
        g_string_append(result, replace);
        // Move the current pointer past the matched search string
        current = match + strlen(search);
    }

    // Append the remaining part of the input
    g_string_append(result, current);

    // Convert GString to gchar* and free the GString
    gchar *output = g_string_free(result, FALSE);
    return output;
}