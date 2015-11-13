#ifndef SWEETEXPR
#define SWEETEXPR

#include "datatypes.h"
#include "charclasses.h"
#include "parser.h"


void traverse_list(swexp_list_node * node,
        void (*callback)(swexp_list_node *));

#endif

