#include "json.h"
#include <assert.h>
#include <string.h>

char *json_strip(char *text)
{
    int quoted = 0;

    char *to_write = text;
    while (*text)
    {
        if (*text == '"' && text[-1] != '\\')
            quoted ^= 1;
        if (text[0] == '/' && text[1] == '/')
        {
            while (text[0] != '\n' && text[0] != '\0')
                ++text;
        }
        if (quoted || (*text != ' ' && *text != '\n'))
            *to_write++ = *text;
        ++text;
    }
    *to_write = '\0';
    return to_write;
}

void json_parser_construct(json_parser *parser, char *text, json_token *tok)
{
    parser->text = text;
    parser->tok = tok;
    parser->it = 0;
}

void json_token_construct(json_token *tok)
{
    tok->type = JSON_UNDEFINED;
    tok->next = tok->value = -1;
}

json_token_id json_parse_object(json_parser *parser);
json_token_id json_parse_array(json_parser *parser);
json_token_id json_parse_primitive(json_parser *parser);

json_token_id json_parse_whatever(json_parser *parser)
{
    switch (parser->text[0])
    {
        case '{':
            return json_parse_object(parser);
        case '[':
            return json_parse_array(parser);
        default:
            return json_parse_primitive(parser);
    }
}

json_token_id json_parse(json_parser *parser)
{
    json_token_id ret = -1;
    if (parser->text[0] == '{')
    {
        ret = json_parse_object(parser);
        if (ret == -1)
            return ret;
    }
    if (parser->text[0] != '\0')
        ret = -1;
    return ret;
}

json_token_id json_parse_prop_chain(json_parser *parser)
{
    json_token_id start = -1,
                  it    = -1,
                  key   = -1;
    it = start = json_parse_primitive(parser);
    if (it == -1)
        return -1;
    if (parser->text[0] != ':')
        return -1;
    ++parser->text;

    key = json_parse_whatever(parser);
    if (key == -1)
        return -1;
    if (parser->tok)
        parser->tok[start].value = key;

    while (parser->text[0] == ',' && parser->text[0] != '}')
    {
        ++parser->text;

        json_token_id jt = json_parse_primitive(parser);
        if (parser->text[0] != ':')
            return -1;
        ++parser->text;

        key = json_parse_whatever(parser);
        if (key == -1)
            return -1;
        if (parser->tok)
        {
            parser->tok[it].next = jt;
            parser->tok[jt].value = key;
        }
        it = jt;
    }
    return start;
}

json_token_id json_parse_object(json_parser *parser)
{
    json_token_id ret = parser->it++;
    if (parser->text[0] != '{')
        return -1;
    ++parser->text;

    if (parser->tok)
    {
        json_token_construct(&parser->tok[ret]);
        parser->tok[ret].type = JSON_OBJECT;
    }

    if (parser->text[0] != '}')
    {
        json_token_id first = json_parse_prop_chain(parser);
        if (parser->tok)
            parser->tok[ret].object.first = first;
    }

    if (parser->text[0] != '}')
        ret = -1;
    ++parser->text;
    return ret;
}

json_token_id json_parse_array(json_parser *parser)
{
    if (parser->text[0] != '[')
        return -1;
    ++parser->text;

    json_token_id ret = parser->it++;

    if (parser->tok)
    {
        json_token *tok = &parser->tok[ret];
        json_token_construct(tok);
        tok->type = JSON_ARRAY;
        tok->array.first = -1;
    }

    json_token_id it = -1;
    if (parser->text[0] != ']')
    {
        json_token_id first = json_parse_whatever(parser);
        if (first == -1)
            return -1;
        it = first;
        if (parser->tok)
            parser->tok[ret].array.first = first;
    }
    while (parser->text[0] == ',')
    {
        ++parser->text;
        json_token_id jt = json_parse_whatever(parser);
        if (parser->tok)
            parser->tok[it].next = jt;
        it = jt;
    }
    if (parser->text[0] != ']')
        return -1;
    ++parser->text;
    return ret;
}

json_token_id json_parse_primitive(json_parser *parser)
{
    switch (parser->text[0])
    {
        case '"':
        {
            char *begin = ++parser->text;
            while (parser->text[0] != '"' || parser->text[-1] == '\\')
                ++parser->text;
            if (parser->tok)
            {
                json_token *tok = &parser->tok[parser->it];
                json_token_construct(tok);
                tok->type = JSON_STRING;
                tok->string = begin;
                parser->text[0] = '\0';
            }
            ++parser->text;
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            int num = 0;
            while ('0' <= parser->text[0] && parser->text[0] <= '9')
            {
                num = 10 * num + ((int)(parser->text[0] - '0'));
                ++parser->text;
            }
            if (parser->tok)
            {
                json_token *tok = &parser->tok[parser->it];
                json_token_construct(tok);
                tok->type = JSON_NUMBER;
                tok->number = num;
            }
            break;
        }
        default:
            return -1;
    }
    return parser->it++;
}

json_token_id json_find_by_key(json_token *tok, json_token_id obj, const char *key)
{
    assert(tok[obj].type == JSON_OBJECT);

    json_token_id it = tok[obj].object.first;
    for (; it != -1 && strcmp(tok[it].string, key); it = tok[it].next)
        ;
    if (it != -1)
        it = tok[it].value;
    return it;
}

json_token_id json_find_by_index(json_token *tok, json_token_id arr, int index)
{
    assert(tok[arr].type == JSON_ARRAY);

    json_token_id it = tok[arr].array.first;
    for (int i = 0; i < index; ++i)
        it = tok[it].next;
    return it;
}