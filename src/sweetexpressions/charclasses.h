#ifndef SWEETEXPR_CHAR_CLASSES
#define SWEETEXPR_CHAR_CLASSES

#include <stdbool.h>
#include <stdio.h>

bool is_space(char c);
bool is_newline(char c);
bool is_opening_brace(char c);
bool is_comment_open(char c);
bool is_comment_close(char c);
bool is_closing_brace(char c);

char brace_pair(char c);

char fpeekc(FILE *f);

#endif

