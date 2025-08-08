#include "response_parser.h"
#include <string.h>

ParsedResponse* response_parser_parse(const gchar *input) {
    if (!input) return NULL;

    ParsedResponse *response = g_new0(ParsedResponse, 1);
    if (!response) return NULL;

    // Simple parsing for <code-block> tags
    const gchar *code_start = strstr(input, "<code-block lang=\"");
    if (code_start) {
        const gchar *lang_start = code_start + strlen("<code-block lang=\"");
        const gchar *lang_end = strchr(lang_start, '"');
        if (lang_end) {
            response->language = g_strndup(lang_start, lang_end - lang_start);
            const gchar *code_content_start = lang_end + 2; // Skip ">
            const gchar *code_end = strstr(code_content_start, "</code-block>");
            if (code_end) {
                response->code = g_strndup(code_content_start, code_end - code_content_start);
                // Assume text is everything before and after code block
                response->text = g_strconcat(
                    g_strndup(input, code_start - input),
                    code_end + strlen("</code-block>"),
                    NULL
                );
            } else {
                response->text = g_strdup(input);
            }
        } else {
            response->text = g_strdup(input);
        }
    } else {
        response->text = g_strdup(input);
    }

    return response;
}

void response_parser_free(ParsedResponse *response) {
    if (!response) return;
    g_free(response->text);
    g_free(response->code);
    g_free(response->language);
    g_free(response);
}