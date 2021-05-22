//node strucutre
typedef enum{red='r', black='b'} nodeColor;

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
