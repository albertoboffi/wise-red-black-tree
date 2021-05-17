#include <stdio.h>
#include "wrbt.h"

node_t *TRoot = NULL,
*TLeaves = NULL,
*TMax = NULL;

//Find the k-th smallest element in the tree with root R
node_t *kthSmallest(node_t *R, int k){
  int pos = R->leftSize+1;
  if (k == pos) return R;
  if (k < pos) return kthSmallest(R->left, k);
  return kthSmallest(R->right, k-pos);
}

//ROTATIONS

//Manage the connection between X's father and Y
static void handleRotFather(node_t *X, node_t *Y){
  if (TRoot == X) TRoot = Y;
  else if (X->p->left == X) X->p->left = Y; //X is the left child of its father
  else X->p->right = Y; //X is the right child of its father
  Y->p = X->p;
}

//Perform a left rotation on a node X
void leftRotate(node_t *X){
  node_t *Y = X->right;

  handleRotFather(X, Y);

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

  handleRotFather(X, Y);

  //connection between X and Y's right subtree
  X->left = Y->right;
  if (X->left != TLeaves) X->left->p = X;

  //connection between X and Y
  Y->right = X;
  X->p = Y;

  X->leftSize -= (Y->leftSize + 1);
}

//Find the node with the minimum key in the tree with root R
node_t *min(node_t *R){
  if (R->left == TLeaves) return R;
  return min(R->left);
}

//Find the inOrder successor of a node X
node_t *inOrderSuccessor(node_t *X){
  if (X->right != TLeaves) return min(X->right); //X has right child
  //X doesn't have a right child
  while (X->p->right == X){ //until X isn't in the left subtree
    X = X->p;
  }
  return X->p;
}
