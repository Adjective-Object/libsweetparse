#include <stdio.h>
#include <stdlib.h>

#include "datatypes.h"

unsigned int indentation = 0;
unsigned int indent_size = 2;
void printList(swexp_list_node * node) {
    while(node != NULL) {
        switch(node->type){
            case ATOM:
                printf("<%s> ", (char *) node->content);
                break;
            case LIST:
                printf("\n");
                for(int i=0; i<indentation; i++){printf(" ");}
                printf("(");
                indentation += indent_size;
                printList(node->content);
                indentation -= indent_size;
                printf(") ");
                break;
            case UNDEFINED:
                printf("{UNDEFINED}");
                break;
        }
        node = node->next;
    }
}

int main(int argc, char ** argv) { 
    FILE * f;

    if(!(f = fopen("example.swexp" ,"r"))) {
        perror("error opening file \n");
    }

    swexp_list_node * head = parse_file_to_atoms(f);
    //traverse_list(head, printExpr);
    printList(head);
    printf("\n");


    return 1;
}
