#ifndef AICODEFRIEND_STRING_UTILS_H
#define AICODEFRIEND_STRING_UTILS_H

#include <glib.h>

gchar* string_utils_trim(const gchar *input);
gchar* string_utils_replace(const gchar *input, const gchar *search, const gchar *replace);

#endif //AICODEFRIEND_STRING_UTILS_H