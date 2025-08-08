#ifndef AICODEFRIEND_RESPONSE_PARSER_H
#define AICODEFRIEND_RESPONSE_PARSER_H

#include <glib.h>

typedef struct {
    gchar *text;
    gchar *code;
    gchar *language;
} ParsedResponse;

ParsedResponse* response_parser_parse(const gchar *input);
void response_parser_free(ParsedResponse *response);

#endif //AICODEFRIEND_RESPONSE_PARSER_H