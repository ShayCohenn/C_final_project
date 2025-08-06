typedef struct node {
  char *name;
  char *content;
  int line;
  struct node *next;
} node;

void add_node_to_ll(node **head, char *name, char *content, int line);
node *search_ll(node *head, char *name, int *found); /*recursive function*/
