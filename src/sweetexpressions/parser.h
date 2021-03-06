#ifndef SWEETEXPR_PARSER
#define SWEETEXPR_PARSER

#include <stdio.h>
#include "datatypes.h"
#include "stream.h"

typedef enum {
  COUNTING_INDENTATION,
  COLLECTING_ATOM,
  COLLECTING_QUOTED_ATOM,
  COLLECTING_LIST,
  SKIP_SPACE,
  DISCARDING_COMMENT,
} parser_state;

typedef struct parser {
  stream *f;
  parser_state state;
  source_location current_location;
  char *buffer;
  unsigned int buffer_size;
  unsigned int buffer_index;
  unsigned int indentation;
} parser;

void begin_atom(parser *p);
void build_atom(parser *p, char c);
swexp_list_node *close_atom(parser *p);

swexp_list_node *parse_s_expr(parser *p, char opening_brace);
swexp_list_node *parse_line(parser *p);
swexp_list_node *parse_block(parser *p);

void free_nodes(swexp_list_node *head);

// parses a file to atoms
swexp_list_node *parse_file_to_atoms(FILE *path, const char *filename,
                                     unsigned int buffsize);

swexp_list_node *parse_memory_to_atoms(const char *f, const char *filename,
                                       size_t memory_length,
                                       unsigned int buffsize);

swexp_list_node *parse_string_to_atoms(const char *f, const char *filename,
                                       unsigned int buffsize);

swexp_list_node *parse_stream_to_atoms(stream *f, const char *filename,
                                       unsigned int buffsize);

#endif
