#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrbt.h"

node_t *TRoot = NULL,
*TLeaves = NULL,
*TMax = NULL;

//Find k-th smallest element, decreasing leftSize field by del_count when moving to the left
static node_t *getKthSmallest(node_t *R, int k, int del_count){
  int pos = R->leftSize+1;
  if (k == pos) return R;
  if (k > pos) return getKthSmallest(R->right, k-pos, del_count);
  R->leftSize -= del_count;
  return getKthSmallest(R->left, k, del_count);
}

//Find k-th smallest element in the tree with root R
node_t *kthSmallest(node_t *R, int k){ return getKthSmallest(R,k,0); }

//Find the node with the minimum key in the tree with root R, decreasing leftSize field by n when moving down
static node_t *getMin(node_t *R, int n){
  if (R->left == TLeaves) return R;
  R->leftSize -= n;
  return getMin(R->left, n);
}

//Find the inOrder successor of X, decreasing leftSize fields when searching for the min
static node_t *getInOrderSuccessor(node_t *X, int n){
  if (X->right != TLeaves) return getMin(X->right, n); //X has right child
  //X doesn't have a right child
  while (X->p->right == X){ //until X isn't in the left subtree
    X = X->p;
  }
  return X->p;
}

//Find the inOrder successor of a node X
node_t *inOrderSuccessor(node_t *X){ return getInOrderSuccessor(X,0); }

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

//INSERTION

//Create sentinel NILs
static void createLeaves(){
  TLeaves = (node_t *)malloc(sizeof(node_t));
  TLeaves->color = black;
}

//Set some attributes of the node after insertion
static void nodeSetup(node_t *X, char *data){
  int data_len;

  X->color = red;
  X->leftSize = 0;
  X->left = TLeaves;
  X->right = TLeaves;
  //copy of the string 'data'
  X->data = (char *)malloc(strlen(data)+1);
  strcpy(X->data, data);
}

//Rebalance tree after an insertion
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

  TMax = node; //Max node update

  nodeSetup(node, data);
  insRebalance(node);
}

//Perform the first descent for a generic insertion
static node_t *insSearch(int key, int m){
  node_t *father, *node = TRoot;
  while (node != TLeaves){
    father = node; //scaling one generation down
    if (father->key > key){
      father->leftSize += m;
      node = father->left;
    }
    else node = father->right;
  }
  return father;
}

//Add the first node to the tree in a generic insertion
node_t *insertFirst(int key, char *data, int m){
  node_t *father, //father of the new node
  *node = (node_t *)malloc(sizeof(node_t));

  //the tree is empty
  if (TRoot == NULL){
    createLeaves();
    node->p = TLeaves;
    TRoot = node;
    TMax = node; //Max node update
  }
  //the tree is not empty
  else{
    if (key > TMax->key){ //the node to insert is the new Max node
      father = TMax; //saving a search
      TMax = node; //Max node update
      father->right = node;
    }
    else{
      father = insSearch(key,m);
      if (father->key > key) father->left = node;
      else father->right = node;
    }
    node->p = father;
  }

  node->key = key;
  nodeSetup(node, data);
  insRebalance(node);

  return node;
}

//Perform a generic insertion of an element of the block follwing the first one
node_t *insertSubsq(node_t *prev, int key, char *data){
  node_t *node = (node_t *)malloc(sizeof(node_t));
  if (prev->right == TLeaves){ //if the right child of the previously inserted node is a leaf
    //prev is the father of the new node
    prev->right = node;
    node->p = prev;
    if (TMax->key < key) TMax = node; //possible update of the max node
  }
  else{
    //prev is the grandpa of the new node
    prev->right->left = node;
    node->p = prev->right;
  }

  node->key = key;
  nodeSetup(node, data);
  insRebalance(node);

  return node;
}

//END INSERTION

//DELETION

//Rebalance tree after a deletion
static void delRebalance(node_t *X, node_t *Y, int n){
  if (X->color == red){ //simple blackness
    X->color = black;
    return;
  }
  //double blackness

  if (X == TRoot) return;

  //X is the left child
  if (X->p->left == X){
    //CASE 1 (X's brother is red)
    if (X->p->right->color == red){
      X->p->right->color = black;
      X->p->color = red;
      if (X->p == Y) X->p->right->leftSize -= n; //leftSize field adjustment
      leftRotate(X->p);
    }
    //CASE 2 (X's brother and X's nephews are black)
    if (X->p->right->left->color == black && X->p->right->right->color == black){
      X->p->right->color = red;
      return delRebalance(X->p, Y, n);
    }
    //CASE 3 (X's brother and X's right nephew are black, X's left nephew is red)
    if (X->p->right->right->color == black){
      X->p->right->color = red;
      X->p->right->left->color = black;
      if (X->p->right->left == Y) X->p->right->leftSize += n; //leftSize field adjustment
      rightRotate(X->p->right);
    }
    //CASE 4 (X's brother is black, X's right nephew is red)
    X->p->right->color = X->p->color;
    X->p->color = black;
    X->p->right->right->color = black;
    if (X->p == Y) X->p->right->leftSize -= n; //leftSize field adjustment
    leftRotate(X->p);
  }

  //X is the right child
  else{
    //CASE 1 (X's brother is red)
    if (X->p->left->color == red){
      X->p->left->color = black;
      X->p->color = red;
      if (X->p->left == Y) X->p->leftSize += n; //leftSize adjustment
      rightRotate(X->p);
    }
    //CASE 2 (X's brother and X's nephews are black)
    if (X->p->left->left->color == black && X->p->left->right->color == black){
      X->p->left->color = red;
      return delRebalance(X->p,Y,n);
    }
    //CASE 3 (X's brother and X's left nephew are black, X's right nephew is red)
    if (X->p->left->left->color == black){
      X->p->left->color = red;
      X->p->left->right->color = black;
      if (X->p->left == Y) X->p->left->right->leftSize -= n; //leftSize adjustment
      leftRotate(X->p->left);
    }
    //CASE 4 (X's brother is black, X's left nephew is red)
    X->p->left->color = X->p->color;
    X->p->color = black;
    X->p->left->left->color = black;
    if (X->p->left == Y) X->p->leftSize += n; //leftSize adjustment
    rightRotate(X->p);
  }
}

//Delete a node X, considering n remaining nodes to delete
static void deleteNode(node_t *X, int n){
  node_t *Y, //X's successor
  *child; //X's child

  if (n>0){
    Y = getInOrderSuccessor(X,1);
    if (X->right == TLeaves) Y->leftSize += n;
  }

  //X has two children
  if (X->left != TLeaves && X->right != TLeaves){
    if (n==0) Y = getInOrderSuccessor(X,1); //if not done before
    //exchange between X and Y
    X->key = Y->key;
    char *data_temp = X->data;
    X->data = Y->data;
    Y->data = data_temp;
    node_t *node_temp = X;
    X = Y;
    Y = node_temp;
  }
  //X has no children
  if (X->left == TLeaves && X->right == TLeaves){
    if (X == TRoot){ //X is the only non-leaf node in the tree
      free(TLeaves);
      TRoot = NULL;
      TMax = NULL;
      TLeaves = NULL;
    }
    else{ //X has a father
      child = TLeaves;
      if (X->p->left == X) X->p->left = child; //X is the left child
      else{ //X is the right child
        if (TMax == X) TMax = X->p; //possible update of the max node
        X->p->right = child;
      }
      child->p = X->p;
    }
  }
  //X has one child
  else{
    if (X->left == TLeaves) child = X->right; //X has only right child
    else{ //X has only left child
      child = X->left;
      if (X == TMax) TMax = child; //possible update of the max node
    }

    if (X == TRoot) TRoot = child; //the tree has only two non-leaf nodes
    else{ //X has a father
      if (X->p->left == X) X->p->left = child; //X is the left child
      else X->p->right = child; //X is the right child
    }
    child->p = X->p;
  }

  if (TRoot != NULL && X->color == black){
    delRebalance(child, Y, n);
    TLeaves->p = NULL; //possible resetting of sentinel NILs
  }

  free(X->data);
  free(X);

  if (n>0) deleteNode(Y,n-1);
}

//Delete the block of size m in position k
void delete(int k, int m){ deleteNode(getKthSmallest(TRoot,k,m), m-1); }

//END DELETION
