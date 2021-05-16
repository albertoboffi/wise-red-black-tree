#include <stdio.h>
#include "wrbt.h"

node_t *TRoot;
node_t *TLeaves;

//Search for the k-th smallest element in the tree with root R
node_t *searchKth(node_t *R, int k){
  int pos = R->leftSize+1;
  if (k == pos) return R;
  if (k < pos) return searchKth(R->left, k);
  return searchKth(R->right, k-pos);
}

//Perform a left rotation on a node X
void leftRotate(node_t *X){
  node_t *Y = X->right;

  //connection between X's father and Y
  if (TRoot == X) TRoot = Y;
  else if (X->p->left == X) X->p->left = Y; //X is the left child of its father
  else X->p->right = Y; //X is the right child of its father
  Y->p = X->p;

  //connection between X and Y's left subtree
  X->right = Y->left;
  if (X->right != TLeaves) X->right->p = X;

  //connection between X and Y
  Y->left = X;
  X->p = Y;

  Y->leftSize += X->leftSize + 1;
}

//Perform a right rotation on a node X
void rightRotate(node_t *X){
  node_t *Y = X->left;

  //connection between X's father and Y
  if (TRoot == X) TRoot = Y;
  else if (X->p->left == X) X->p->left = Y;
  else X->p->right = Y;
  Y->p = X->p;

  //connection between X and Y's right subtree
  X->left = Y->right;
  if (X->left != TLeaves) X->left->p = X;

  //connection between X and Y
  Y->right = X;
  X->p = Y;

  X->leftSize -= (Y->leftSize + 1);
}
