#ifndef SWEETEXPR
#define SWEETEXPR

#include "sweetexpressions/datatypes.h"
#include "sweetexpressions/charclasses.h"
#include "sweetexpressions/parser.h"


void traverse_list(swexp_list_node * node,
        void (*callback)(swexp_list_node *));

void free_list(swexp_list_node * head);

void free_node(swexp_list_node * node);
void free_node_nonrecursive(swexp_list_node * node);

void print_list(swexp_list_node * node);

#endif

