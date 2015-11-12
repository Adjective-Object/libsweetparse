#ifndef SWEETPARSE
#define SWEETPARSE

typedef enum {
    UNDEFINED,
    LIST,
    ATOM
} swexp_type;

typedef struct swexp_list_node {
    swexp_type type;
    void * content;
    struct swexp_list_node * next;
} swexp_list_node;

typedef enum {
    COUNTING_INDENTATION,
    COLLECTING_ATOM,
    COLLECTING_QUOTED_ATOM,
    COLLECTING_LIST,
    SKIP_SPACE
} parser_state;

typedef struct parse{
    FILE *f;
    parser_state state;
    char * buffer;
    unsigned int buffer_index;
    unsigned int indentation;
    unsigned int symbol_ct_this_line;
    unsigned int linect;
} parse;

swexp_list_node * parse_file_to_atoms(FILE * fd);
swexp_list_node * parse_line(parse * p);
swexp_list_node * parse_s_expression(parse * p);
void traverse_list(swexp_list_node * node, 
        void (*callback)(swexp_list_node *));

#endif
