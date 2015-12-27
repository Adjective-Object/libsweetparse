#include "stdlib.h"

#include "sweetexpressions.h"
#include "sweetexpressions/charclasses.h"
#include "sweetexpressions/datatypes.h"

void traverse_list(swexp_list_node * node,
        void (*callback)(swexp_list_node *)) {
    callback(node);
    if(node != NULL && node->type == LIST) {
        traverse_list(
                (swexp_list_node *) node->content,
                callback);
    }
    
    if (node != NULL) {
        traverse_list(node->next, callback);
    }
}

void free_list(swexp_list_node * node) {
    swexp_list_node * next;
    while (node != NULL) {
        next = node->next;

        if(node->type == LIST)
            free_list((swexp_list_node *) node->content);
        else
            free(node->content);

        free(node);
        node = next;
    }
}
