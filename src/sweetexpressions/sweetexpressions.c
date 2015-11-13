#include "charclasses.h"
#include "datatypes.h"

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


