#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "sweetexpressions.h"

unsigned int indentation = 0;
unsigned int INDENT_SIZE = 2;

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
                indentation += INDENT_SIZE;
                printList(node->content);
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

static void print_usage(const char * programname) {
    printf("%s -i <indent_size> <filenames ...>\n", programname);
}

static int parse_args(int argc, char ** argv) {
    // get option arguments into global config variables
    // return the index of the first non-flag arg
    char c; 
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch(c) {
            case 'i':
                INDENT_SIZE = sscanf("%d", optarg);
                break;
            default:
                printf("unrecognized argument '%c'\n", c);
                print_usage(argv[0]);
                exit(1);
        }
    }
    return optind;
}

int main(int argc, char ** argv) { 
    FILE * f;
    int i = parse_args(argc, argv);

    if (i == argc) {
        print_usage(argv[0]);
        return 1;
    }

    for (; i<argc; i++) {
        if(!(f = fopen(argv[i], "r"))) {
            printf("error opening file '%s'\n", argv[i]);
            exit(1);
        }

        swexp_list_node * head = parse_file_to_atoms(f, 255);
        printList(head);
        printf("\n");
    }

    return 0;
}
