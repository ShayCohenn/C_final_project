#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/linked_list_handler.h"
#include "../headers/utils.h"
#include "../headers/errors.h"

node *make_node(char *name, char *content, int line) {
  node *temp;
  temp = handle_malloc(sizeof(node));
  
  temp->name = name;
  temp->content = content;
  temp->line = line;
  temp->next = NULL;
  
  return temp;
}

node *search_ll(node *head, char *name, int *found) {
  *found = 0;
  
  if(head == NULL) return NULL;
  
  if(strcmp(name, head->name) == 0) {
    *found = 1;
    printf("%s already exists in this linked list", name);
    return head;
  }
  
  if(head->next == NULL) return head;
  
  return search_ll(head->next, name, found);
}

node *create_node(char *name, char *content, int line) {
  node *temp;
  temp = handle_malloc(sizeof(node));
  temp->name = name;
  temp->content = content;
  temp->line = line;
  temp->next = NULL;
  
  return temp;
}

void add_node_to_ll(node **head, char *name, char *content, int line) {
  int found;
  node *new_node, *temp;
  found = 0;
  
  temp = search_ll(*head, name, &found);
  
  if(found && strcmp(temp->content, content) != 0) {
    report_internal_error(ERROR_CODE_10);
    free(name);
    free(content);
    return;
  }
  
  if(!found) {
    new_node = create_node(name, content, line);
    
    if(temp == NULL)
      *head = new_node;
    else
      temp->next = new_node;
  }
}

void remove_node_from_ll(node *node01) {
  free(node01->name);
  free(node01->content);
  free(node01);
}

void delete_ll(node *head) {
  while(head != NULL) {
    node *temp = head;
    head = head->next;
    remove_node_from_ll(temp);
  }
}
