typedef struct node {
  char *name;
  char *content;
  int line;
  struct node *next;
} node;

void add_node_to_ll(node **head, char *name, char *content, int line);
void remove_node_from_ll(node *node01);
node *search_ll(node *head, char *name, int *found); /*recursive function*/
node *create_node(char *name, char *content, int line);
void delete_ll(node *head);
