#include <stdio.h>
#include "wrbt.h"

//search for the k-th smallest element in the tree with root R
node_t *searchKth(node_t *R, int k){
  int pos = R->leftSize+1;
  if (k == pos) return R;
  if (k < pos) return searchKth(R->left, k);
  return searchKth(R->right, k-pos);
}
