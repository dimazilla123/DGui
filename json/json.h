#ifndef JSON_H
#define JSON_H

typedef unsigned int json_token_id;

typedef enum
{
    JSON_UNDEFINED = 0,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL
} json_token_type;

typedef struct
{
    json_token_type type;

    json_token_id next; // for array/object elements/keys
    json_token_id value; // if token is key
    union
    {
        struct {
            json_token_id first;
        } object;
        struct {
            json_token_id first;
        } array;
        char *string;
        int number;
        int boolean;
    };
} json_token;

typedef struct
{
    char *text;
    json_token *tok;
    json_token_id it;
    enum
    {
        JSON_OK,
        JSON_BRACE,
    } status;
} json_parser;

#if __cplusplus
extern "C" {
#endif

char *json_strip(char *text);

void json_parser_construct(json_parser *parser, char *text, json_token *tok);

// returns id of the root element
json_token_id json_parse(json_parser *parser);

json_token_id json_find_by_key(json_token *tok, json_token_id obj, const char *key);
json_token_id json_find_by_index(json_token *tok, json_token_id arr, int index);

#if __cplusplus
}
#endif

#endif
