//node strucutre
typedef enum{red='0', black='0'} nodeColor;

typedef struct node{
  int key;
  nodeColor color;
  struct node *left;
  struct node *right;
  struct node *p;
  int leftSize;

  char *data;
}node_t;

//operations
