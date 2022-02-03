#include "pretty_json.h"

void json_pretty_print(FILE *out, json_token *tok, json_token_id root, int depth)
{
    switch (tok[root].type)
    {
        case JSON_OBJECT:
        {
            fprintf(out, "{\n");
            json_token_id it = tok[root].object.first;
            for (; it != -1; it = tok[it].next)
            {
                for (int i = 0; i < depth + 1; ++i)
                    fprintf(out, "    ");
                json_pretty_print(out, tok, it, depth + 1);
                fprintf(out, ": ");
                json_pretty_print(out, tok, tok[it].value, depth + 1);
                fprintf(out, ",\n");
            }
            for (int i = 0; i < depth; ++i)
                fprintf(out, "    ");
            fprintf(out, "}");
            return;
        }
        case JSON_ARRAY:
        {
            fprintf(out, "[\n");
            json_token_id it = tok[root].array.first;
            for (; it != -1; it = tok[it].next)
            {
                for (int i = 0; i < depth + 1; ++i)
                    fprintf(out, "    ");
                json_pretty_print(out, tok, it, depth + 1);
                fprintf(out, ",\n");
            }
            for (int i = 0; i < depth; ++i)
                fprintf(out, "    ");
            fprintf(out, "]");
            return;
        }
        case JSON_STRING:
        {
            fprintf(out, "\"%s\"", tok[root].string);
            return;
        }
        case JSON_NUMBER:
        {
            fprintf(out, "%d", tok[root].number);
            return;
        }
        default:
        {
            fprintf(out, "ERROR\n");
            return;
        }
    }
}