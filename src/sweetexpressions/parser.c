#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "charclasses.h"
#include "datatypes.h"
#include "parser.h"

char pgetc(parser *p) {
    char c = sgetc(p->f);
    if (is_newline(c)) {
        p->current_location.column = 1;
        p->current_location.line++;
    } else {
        p->current_location.column++;
    }
    return c;
}

void prewind(parser * p, char c) {
    sseek(p->f, -1, SEEK_CUR);
    if (is_newline(c)) {
        p->current_location.line--;
    } else {
        p->current_location.column--;
    }
}

void begin_atom(parser * p) {
    p->buffer_index = 0;
    p->state = COLLECTING_ATOM;
}

void build_atom(parser * p, char c) {
    if (p->buffer_index > p->buffer_size) {
        printf("buffer overflow \"%*s\"", p->buffer_size, p->buffer);
    }
    p->buffer[p->buffer_index] = c;
    p->buffer_index++;
}

swexp_list_node * close_atom(parser * p) {
    swexp_list_node * node = malloc(sizeof(swexp_list_node));
    char * content = malloc(p->buffer_index + 1);
    memcpy(content, p->buffer, sizeof(char) * p->buffer_index);
    content[p->buffer_index] = '\0';

    node->content = content;
    node->type = ATOM;
    node->next = NULL;

    node->location = (source_location *) malloc(sizeof(source_location));
    node->location->source_file_name = p->current_location.source_file_name;
    node->location->line = p->current_location.line;
    node->location->column = p->current_location.column;

    p->state = SKIP_SPACE;
    return node;
}

#define IGNORE_COMMENTS() \
        if (is_comment_open(c)) { \
            do {} while (! is_comment_close((c = pgetc(p))) ); \
            prewind(p, c); \
            continue; \
        }



swexp_list_node * parse_s_expr(parser * p, char opening_brace) {
    char c;

    swexp_list_node fakehead, *tail=&fakehead;
    fakehead.next = NULL;
    fakehead.content = NULL;
    fakehead.type = UNDEFINED;

    char closing_brace = brace_pair(opening_brace);

    while((c = pgetc(p)) != EOF && !is_closing_brace(c)) {
        // if we encounter a comment in any state, strip it out
        IGNORE_COMMENTS()

        switch(p->state) {
            case SKIP_SPACE:
                if (is_space(c) || is_newline(c)) {
                    // do nothing if it is space or newline
                }
                else if (is_opening_brace(c)) {
                    // parse the parenthesized s expression into a list
                    // and append it to the thing
                   swexp_list_node *list = 
                       malloc(sizeof(swexp_list_node));
                   list->type = LIST;
                   list->next = NULL;
                   list->location = NULL;
                   list->content = parse_s_expr(p, c);

                   tail->next = list;
                   tail = list;
                } else {
                    // step back and start collecting the atom
                    prewind(p, c);
                    begin_atom(p);
                }
                break;
            case COLLECTING_ATOM:
                if (is_space(c) || is_newline(c)) {
                    prewind(p, c);
                    tail->next = close_atom(p);
                    tail = tail->next;
                } else if (is_opening_brace(c)) {
                    swexp_list_node * node = close_atom(p);
                    node->next = parse_s_expr(p, c);
                    tail->next = listof(node);
                    tail = chain_tail(tail);
                } else {
                    build_atom(p, c);
                }
                break;
            default:
                printf("unexpected state %d in parse_s_expr", p->state);
                exit(1);
        }
    }

    if(is_closing_brace(c) && c != closing_brace) {
        printf("mismatched braces in s expression\n");
        exit(1);
    }

    if (p->state == COLLECTING_ATOM) {
        tail->next = close_atom(p);
    }

    if (c == EOF) {
        printf("unexpected EOF while parsing s expression\n");
        exit(1);
    }

    p->state = SKIP_SPACE;

    return fakehead.next;
    
}

swexp_list_node * parse_line(parser * p) {
    // parses a line of text, starting at a non-whitespace char
    char c;

    // build a list of expressions started by this
    // list head on the stack.
    swexp_list_node head, *tail;
    head.next = NULL;
    head.content = NULL;
    head.type = UNDEFINED;
    tail = &head;

    p->state = SKIP_SPACE;

    while((c = pgetc(p)) != EOF 
            && !is_newline(c)
            && !is_closing_brace(c)) {
        // if we encounter a comment in any state, strip it out
        IGNORE_COMMENTS()

        switch(p->state) {
            case COLLECTING_ATOM:
                if (is_space(c)) {
                    // end atom
                    tail->next = close_atom(p);
                    tail = tail->next;
                    prewind(p, c);
                } else if (is_opening_brace(c)) {
                    swexp_list_node * bracehead = close_atom(p);
                    swexp_list_node * bracecontent =
                        parse_s_expr(p, c);
                    bracehead->next = bracecontent;
                    tail->next = listof(bracehead);
                    tail = chain_tail(tail);
                } else {
                    // continue to build item
                    build_atom(p, c); 
                }
                break;
            case SKIP_SPACE:
                if(is_opening_brace(c)) {
                    swexp_list_node * brace =
                        parse_s_expr(p, c);
                    tail->next = brace;
                    tail = chain_tail(tail);
                } else if (!is_space(c)) {
                    begin_atom(p);
                    prewind(p, c);
                }
                break;
            default:
                printf("unexpected state %d in parse_line\n", p->state);
                exit(1);
        } 
    }

    if(is_newline(c)){p->indentation = 0;}
    if(is_closing_brace(c)){
        printf("encountered unmatched closing brace\n");
        exit(1);
    }

    // close ongoing capture
    if(p->state == COLLECTING_ATOM) {
        tail->next = close_atom(p);
    }

    // if the number of collected atoms is more than one,
    // make it a list and return it
    if(chain_len(head.next) > 1) {
        swexp_list_node * listhead = malloc(sizeof(swexp_list_node));
        listhead->type = LIST;
        listhead->next = NULL;
        listhead->content = head.next;
        listhead->location = NULL;
        return listhead;
    } else {
        return head.next;
    }
}

swexp_list_node * parse_block(parser * p) {
    // parses a block of lines with the same indentation into a chain
    // (not a list)
    char c;

    swexp_list_node fakehead, *tail;
    fakehead.next = NULL;
    fakehead.content = NULL;
    fakehead.type = UNDEFINED;
    tail = &fakehead;

    // get initial indentation by consuming characters until we find some
    unsigned int current_indentation;
    for (current_indentation = p->indentation;
            is_space(pgetc(p));
            current_indentation++){}
    prewind(p, '\0');

    while((c = pgetc(p)) != EOF) {
        // if we encounter a comment in any state, strip it out
        IGNORE_COMMENTS()

        switch(p->state) {
            case COUNTING_INDENTATION:
                if (is_space(c)) {p->indentation++;}
                else if (is_newline(c)){p->indentation = 0;}
                else {
                    // this is a start of an atom.
                    // parse as appropriate based on indent
                    prewind(p, c);
                    if (p->indentation > current_indentation) {
                        if (tail->type == ATOM) {
                            // make the tail a list before appending
                            swexp_list_node * tailcont = 
                                malloc(sizeof(swexp_list_node));
                            tailcont->type = ATOM;
                            tailcont->next = NULL;
                            tailcont->content = tail->content;
                            tailcont->location = NULL;

                            tail->type = LIST;
                            tail->content = tailcont;
                            if (tail->location != NULL) {
                                free(tail->location);
                                tail->location = NULL;
                            }
                        }
                        // append it to the list that is the last
                        // element
                        list_tail(tail)->next = parse_block(p);
                    } else if (p->indentation == current_indentation) {
                        tail->next = parse_line(p);
                         p->state = COUNTING_INDENTATION;
                        tail = tail->next;
                    } else {
                        goto clean_and_return;
                    }
                }
                break;
            default:
                printf("unexpected state in parse_line\n");
                exit(1);
        }
    }

    clean_and_return:
    return fakehead.next;
}

swexp_list_node * parse_stream_to_atoms(
        stream *f, const char * filename, unsigned int buffsize) {
    char buffer[buffsize];
    parser p = {
        .f = f,
        .state = COUNTING_INDENTATION, 
        .current_location = {
            .source_file_name = filename,
            .line = 1,
            .column = 1,
        },
        .buffer = (char*) &buffer,
        .buffer_size = 255,
        .buffer_index = 0,
        .indentation = 0
    };

    swexp_list_node * container = malloc(sizeof(swexp_list_node));
    container->type = LIST;
    container->next = NULL;
    container->content = parse_block(&p);
    container->location = NULL;
    return container;
}

swexp_list_node * parse_file_to_atoms(FILE * f, const char * filename, unsigned int buffsize) {
    stream s;
    s.type = __SWEXP_FROM_FILE;
    s.file = f;
    s.origin = NULL;
    s.current = NULL;
    s.buflen = 0;
    swexp_list_node * nodes = parse_stream_to_atoms(&s, filename, buffsize);
    return nodes;
}

swexp_list_node * parse_memory_to_atoms(
        const char * f,
        const char * filename,
        size_t memory_length,
        unsigned int buffsize) {
    stream s;
    s.type = __SWEXP_FROM_MEMORY;
    s.file = NULL;
    s.buflen = memory_length;
    s.origin = f;
    s.current = f;

    return parse_stream_to_atoms(&s, filename, buffsize);
}

swexp_list_node * parse_string_to_atoms(
        const char * f,
        const char * filename,
        unsigned int buffsize) {
    return parse_memory_to_atoms(f, filename, strlen(f), buffsize);
}




