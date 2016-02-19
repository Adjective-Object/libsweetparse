#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "charclasses.h"

bool is_space(char c) { return c == '\t' || c == ' '; }

bool is_comment_open(char c) { return c == ';'; }

bool is_comment_close(char c) { return is_newline(c); }

bool is_newline(char c) { return c == '\n' || c == '\r'; }

bool is_opening_brace(char c) { return c == '{' || c == '[' || c == '('; }

bool is_closing_brace(char c) { return c == '}' || c == ']' || c == ')'; }

char brace_pair(char c) {
  switch (c) {
  case '{':
    return '}';
  case '(':
    return ')';
  case '[':
    return ']';
  default:
    printf("error trying to find brace pair for '%c'.\n", c);
    exit(1);
  }
}

char fpeekc(FILE *f) {
  char c = fgetc(f);
  ungetc(c, f);
  return c;
}
