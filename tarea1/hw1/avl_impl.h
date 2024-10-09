#ifndef AVL_IMPL_H
#define AVL_IMPL_H

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "avl.h"

// AVLNode

template <typename T>
AVLNode<T>::AVLNode(T keyue)
    : data(keyue), left(nullptr), right(nullptr), height(0) {}

// AVLTree

template <typename T>
AVLTree<T>::AVLTree() : root(nullptr) {}

template <typename T>
int AVLTree<T>::height(AVLNode<T>* node) {
    if (node == nullptr){ return -1;}
    return node->height;
}

template <typename T>
int AVLTree<T>::getBalance(AVLNode<T>* node) {
    if (node == nullptr){ return 0;}
    return height(node->left) - height(node->right);
}

template <typename T>
AVLNode<T>* AVLTree<T>::rightRotate(AVLNode<T>* y) {
    AVLNode<T>* x = y->left;
    AVLNode<T>* a = x->right;

    x->right = y;
    y->left = a;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}


// Rotación izquierda
template <typename T>
AVLNode<T>* AVLTree<T>::leftRotate(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    AVLNode<T>* a = y->left;

    y->left = x;
    x->right = a;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}


template <typename T>
AVLNode<T>* AVLTree<T>::minValueNode(AVLNode<T>* node) {
    AVLNode<T>* current = node;
    while (current->left != nullptr) {current = current->left;}
    return current;
}


// Inserción
template <typename T>
AVLNode<T>* AVLTree<T>::insert(AVLNode<T>* node, T key) {
    if (node == nullptr)
        return new AVLNode<T>(key);

    if (key < node->data) {node->left = insert(node->left, key);}
    else if (key > node->data) {node->right = insert(node->right, key);}
    else {return node;}

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->data) {return rightRotate(node);} 
    else if (balance < -1 && key > node->right->data) { return leftRotate(node);} 
    else if (balance > 1 && key > node->left->data) {
      node->left = leftRotate(node->left);
      return rightRotate(node);
    } else if (balance < -1 && key < node->right->data) {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }

    return node;
}


// Eliminar un nodo
template <typename T>
AVLNode<T>* AVLTree<T>::remove(AVLNode<T>* root, T key) {
    if (root == nullptr) {return root;}

    if (key < root->data) {root->left = remove(root->left, key);}
    else if (key > root->data) {root->right = remove(root->right, key);}
    else {
        if (root->left == nullptr || root->right == nullptr) {
            AVLNode<T>* temp;
            if (root->left) {temp = root->left;} 
            else {temp = root->right;}

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {*root = *temp;}
            delete temp;
        } else {
            AVLNode<T>* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = remove(root->right, temp->data);
        }
    }

    if (root == nullptr){ return root;}

    root->height = std::max(height(root->left), height(root->right)) + 1;

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0){return rightRotate(root);}  
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0){return leftRotate(root);}   
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}


// Búsqueda
template <typename T>
bool AVLTree<T>::search(AVLNode<T>* node, T key) {
    if (node == nullptr){return false;}
    if (key == node->data){ return true;}
    else if (key < node->data){ return search(node->left, key);}
    else{ return search(node->right, key);} 
}


// Recorrido preorder
template <typename T>
void AVLTree<T>::preorder(AVLNode<T>* root, std::vector<T>& ret) {
    if (root != nullptr) {
        ret.push_back(root->data);
        preorder(root->left, ret);
        preorder(root->right, ret);
    }
}

// Recorrido inorder
template <typename T>
void AVLTree<T>::inorder(AVLNode<T>* root, std::vector<T>& ret) {
    if (root != nullptr) {
        inorder(root->left, ret);
        ret.push_back(root->data);
        inorder(root->right, ret);
    }
}


// Recorrido postorder
template <typename T>
void AVLTree<T>::postorder(AVLNode<T>* root, std::vector<T>& ret) {
    if (root != nullptr) {
        postorder(root->left, ret);
        postorder(root->right, ret);
        ret.push_back(root->data);
    }
}


/// Public functions
template <typename T>
void AVLTree<T>::insert(T key) {
    root = insert(root, key);
}

template <typename T>
void AVLTree<T>::remove(T key) {
    root = remove(root, key);
}

template <typename T>
bool AVLTree<T>::search(T key) {
    return search(root, key);
}

template <typename T>
std::vector<T> AVLTree<T>::preorderTraversal() {
    std::vector<T> ret;
    preorder(root, ret);
    return ret;
}

template <typename T>
std::vector<T> AVLTree<T>::inorderTraversal() {
    std::vector<T> ret;
    inorder(root, ret);
    return ret;
}

template <typename T>
std::vector<T> AVLTree<T>::postorderTraversal() {
    std::vector<T> ret;
    postorder(root, ret);
    return ret;
}

template <typename T>
int AVLTree<T>::height() {
  if (root == nullptr) { return 0; }
  return root->height; 
}

template <typename T>
bool AVLTree<T>::isBalanced() {
  int balance = getBalance(root);
  if (balance >= -1 && balance <= 1) {return true;} 
  else {return false;} 
}



#endif
