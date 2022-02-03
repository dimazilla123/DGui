#ifndef PRETTY_JSON_H
#define PRETTY_JSON_H

#include <stdio.h>
#include "json.h"

void json_pretty_print(FILE *out, json_token *tok, json_token_id root, int depth);

#endif