#include "json.h"

#include <stdio.h>
#include <stdlib.h>

#include "pretty_json.h"

char *readcontent(FILE *in, size_t *textsz_p)
{
    fseek(in, 0L, SEEK_END);
    *textsz_p = ftell(in);
    rewind(in);
    char *text = calloc(*textsz_p + 1, sizeof(text[0]));
    if (!text)
    {
        printf("Cannot allocate memory for text from file!\n");
        return NULL;
    }
    fread(text, *textsz_p, sizeof(char), in);
    return text;
}

char base_text[] = "{\"a \\\" a\":1,\"b\":2,\"c\":{\"d\":4},5: [1,2,3,4], 6:[]}";

int main(int argc, char const *argv[])
{

    char *text = base_text;
    if (argc >= 2)
    {
        FILE *in = fopen(argv[1], "r");
        if (!in)
        {
            perror("Cannot open input file");
            return -1;
        }
        size_t sz = 0;
        text = readcontent(in, &sz);
    }
    printf("Original\n");
    printf("%s\n", text);

    printf("Striped\n");
    json_strip(text);
    printf("%s\n", text);

    json_parser pars;
    json_parser_construct(&pars, text, NULL);

    int root = json_parse(&pars);
    printf("root = %d\n", root);

    if (root != -1)
    {
        json_token *tok = (json_token*)calloc(pars.it, sizeof(*tok));
        json_parser_construct(&pars, text, tok);
        root = json_parse(&pars);

        printf("root = %d\n", root);

        json_pretty_print(stdout, tok, root, 0);

        free(tok);
    }

    if (argc >= 2)
        free(text);

    return 0;
}