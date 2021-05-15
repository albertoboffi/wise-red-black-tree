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
/*
Input:
  - R => Root of the (sub)tree
  - k => Position of the element you're looking for

Output:
  k-th smallest element of the subtree with root R
*/
node_t *searchKth(node_t *R, int k);
