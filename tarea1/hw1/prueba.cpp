#include "avl.h" // Assuming your AVLTree implementation is in avl.h
#include "avl_impl.h"
#include <iostream>

int main() {
    AVLTree<int> tree;

    // Inserting elements as in the test case
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    // Print the inorder traversal (expected to be unbalanced)
    std::cout << "Inorder traversal (before fix): ";
    std::vector<int> traversal = tree.inorderTraversal();
    for (int num : traversal) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Fix the insertion logic (modify avl.h accordingly)

    // Re-insert elements to demonstrate fixed behavior
    tree.clear(); // Clear the existing unbalanced tree
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    // Print the inorder traversal (expected to be balanced)
    std::cout << "Inorder traversal (after fix): ";
    traversal = tree.inorderTraversal();
    for (int num : traversal) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}