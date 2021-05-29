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

extern node_t *TRoot, *TLeaves;
node_t *kthSmallest(node_t *R, int k);
node_t *inOrderSuccessor(node_t *X);
void append(char *data);
node_t *insertFirst(int key, char *data, int m);
node_t *insertSubsq(node_t *prev, int key, char *data);
void delete(int k, int m);
