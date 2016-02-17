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

        if(node->location != NULL)
            free(node->location);

        free(node);
        node = next;
    }

}

void free_node(swexp_list_node * node) {
    if(node->type == LIST)
        free_list((swexp_list_node *) node->content);
    else
        free(node->content);

    if(node->location != NULL)
        free(node->location);

    free(node);
}

void free_node_nonrecursive(swexp_list_node * node) {
    if(node->type != LIST)
        free(node->content);

    if(node->location != NULL)
        free(node->location);
    free(node);
}




#define INDENT_SIZE 4

void _print_list(int indentation, swexp_list_node * node) {
    while(node != NULL) {
        switch(node->type){
            case ATOM:
                printf("<%s> ", (char *) node->content);
                break;
            case LIST:
                printf("\n");
                for(int i=0; i<indentation; i++){printf(" ");}
                printf("(");
                indentation += INDENT_SIZE;
                _print_list(indentation, node->content);
                indentation -= INDENT_SIZE;
                printf(") ");
                break;
            case UNDEFINED:
                printf("{UNDEFINED}");
                break;
        }
        node = node->next;
    }
}

void print_list(swexp_list_node * node) {
    _print_list(0, node);
}


