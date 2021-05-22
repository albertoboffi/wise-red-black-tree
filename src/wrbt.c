#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

//END ROTATIONS

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

//INSERTION

//Create sentinel NILs
static void createLeaves(){
  TLeaves = (node_t *)malloc(sizeof(node_t));
  TLeaves->key = -1;
  TLeaves->color = black;
}

static void insRebalance(node_t *X){
  //X is the root
  if (X == TRoot){
    X->color = black;
    return;
  }

  //X's father is black
  if (X->p->color == black) return;

  //X's father is the left child
  if (X->p == X->p->p->left){
    //CASE 1 (X's uncle is red)
    if (X->p->p->right->color == red){
      X->p->color = black;
      X->p->p->right->color = black;
      X->p->p->color = red;
      insRebalance(X->p->p);
    }
    else{
      //CASE 2 (X's uncle is black, X is the right child)
      if (X == X->p->right){
        X = X->p;
        leftRotate(X);
      }
      //CASE 3 (X's uncle is black, X is the left child)
      X->p->color = black;
      X->p->p->color = red;
      rightRotate(X->p->p);
    }
  }

  //X's father is the right child
  else{
    //CASE 1 (X's uncle is red)
    if (X->p->p->left->color == red){
      X->p->color = black;
      X->p->p->left->color = black;
      X->p->p->color = red;
      insRebalance(X->p->p);
    }
    else{
      //CASE 2 (X's uncle is black, X is the left child)
      if (X == X->p->left){
        X = X->p;
        rightRotate(X);
      }
      //CASE 3 (X's uncle is black, X is the right child)
      X->p->color = black;
      X->p->p->color = red;
      leftRotate(X->p->p);
    }
  }
}

//Append an element to the tree
void append(char *data){
  node_t *node = (node_t *)malloc(sizeof(node_t));
  int data_len;

  if (TRoot==NULL){ //the tree is empty
    createLeaves();
    node->key = 1;
    node->p = TLeaves;
    TRoot = node; //Root update
  }
  else{
    node->key = TMax->key + 1;
    //connection with TMax
    TMax->right = node;
    node->p = TMax;
  }

  //setting of remaining fields
  node->color = red;
  node->leftSize = 0;
  node->left = TLeaves;
  node->right = TLeaves;
  //copy of the string 'data'
  node->data = (char *)malloc(strlen(data)+1);
  strcpy(node->data, data);

  TMax = node; //Max node update

  insRebalance(node);
}

//Perform the first descent for a generic insertion
node_t *insSearch(int key, int m){
  node_t *father, *node = TRoot;
  while (node->key > -1){
    father = node; //scaling one generation down
    if (father->key > key){
      father->leftSize += m;
      node = father->left;
    }
    else node = father->right;
  }
  return father;
}

//END INSERTION
