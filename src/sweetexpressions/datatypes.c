#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "datatypes.h"
#include "charclasses.h"
#include "parser.h"

unsigned int list_len(swexp_list_node *node) {
  if (node->type != LIST) {
    printf("list_len called on non-list node\n");
    exit(1);
  }
  return chain_len((swexp_list_node *)node->content);
}

unsigned int chain_len(swexp_list_node *node) {
  unsigned int count = 0;
  while (node != NULL) {
    node = node->next;
    count++;
  }
  return count;
}

swexp_list_node *list_head(swexp_list_node *list) {
  if (list->type != LIST) {
    printf("tried to get head of non list\n");
    printf("%s\n", (char *)list->content);
    exit(1);
  }

  return (swexp_list_node *)list->content;
}

swexp_list_node *chain_tail(swexp_list_node *list) {
  for (; list->next != NULL; list = list->next) {
  }
  return list;
}

swexp_list_node *list_tail(swexp_list_node *list) {
  if (list->type != LIST) {
    printf("tried to get tail of non list\n");
    printf("%s\n", (char *)list->content);
    exit(1);
  }

  return chain_tail((swexp_list_node *)list->content);
}

swexp_list_node *listof(swexp_list_node *list_content) {
  swexp_list_node *list = malloc(sizeof(swexp_list_node));
  list->next = NULL;
  list->type = LIST;
  list->content = list_content;
  list->location = NULL;
  return list;
}
