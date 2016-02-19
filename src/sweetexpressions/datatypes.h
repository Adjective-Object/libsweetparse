#ifndef SWEETEXPR_DATATYPES
#define SWEETEXPR_DATATYPES

typedef struct source_location {
  const char *source_file_name;
  size_t line;
  size_t column;
} source_location;

typedef enum { UNDEFINED, LIST, ATOM } swexp_type;

typedef struct swexp_list_node {
  swexp_type type;
  void *content;
  struct swexp_list_node *next;
  source_location *location;
} swexp_list_node;

// get the length of a list
unsigned int chain_len(swexp_list_node *list);
unsigned int list_len(swexp_list_node *list);

// step to the tail of a chain / list
swexp_list_node *chain_tail(swexp_list_node *list);
swexp_list_node *list_head(swexp_list_node *list);
swexp_list_node *list_tail(swexp_list_node *list);

// build a list around a node
swexp_list_node *listof(swexp_list_node *list_content);
swexp_list_node *atomof(char *c);

#endif
