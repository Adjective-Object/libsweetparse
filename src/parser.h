#ifndef SWEETEXPR_PARSER
#define SWEETEXPR_PARSER

#include <stdio.h>
#include "datatypes.h"

typedef enum {
    COUNTING_INDENTATION,
    COLLECTING_ATOM,
    COLLECTING_QUOTED_ATOM,
    COLLECTING_LIST,
    SKIP_SPACE
} parser_state;

typedef struct parser {
    FILE *f;
    parser_state state;
    char * buffer;
    unsigned int buffer_size;
    unsigned int buffer_index;
    unsigned int indentation;
    unsigned int linect;
} parser;

void begin_atom(parser *p);
void build_atom(parser *p, char c);
swexp_list_node * close_atom(parser *p);

swexp_list_node * parse_s_expr(parser * p, char opening_brace);
swexp_list_node * parse_line(parser * p);
swexp_list_node * parse_block(parser * p);

// parses a file to atoms
swexp_list_node * parse_file_to_atoms(
        FILE* f, unsigned int buffsize);

#endif

