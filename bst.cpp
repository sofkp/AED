#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

template <typename T>
struct Node {
    T key;
    Node* left;
    Node* right;
    Node(T val) : key(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BST {
private:
    Node<T>* root;

    Node<T>* insert(Node<T>* node, T key) {
        if (!node) return new Node<T>(key);
        if (key < node->key)
            node->left = insert(node->left, key);
        else
            node->right = insert(node->right, key);
        return node;
    }

    Node<T>* deleteNode(Node<T>* current, T value) {
        if (current == nullptr) return current;
        if (value < current->key) {
            current->left = deleteNode(current->left, value);
        } else if (value > current->key) {
            current->right = deleteNode(current->right, value);
        }
        else {
            // No children
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }
                // One child
            else if (current->left == nullptr) {
                Node<T>* temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                Node<T>* temp = current->left;
                delete current;
                return temp;
            }
                // Two children
            else {
                Node<T>* tempParent = current;
                Node<T>* temp = current->left;

                while (temp->right != nullptr) {
                    tempParent = temp;
                    temp = temp->right;
                }

                if (tempParent == current) {
                    temp->right = current->right;
                } else {
                    tempParent->right = temp->left;
                    temp->left = current->left;
                    temp->right = current->right;
                }

                delete current;
                return temp;
            }
        }

        return current;
    }

    Node<T>* search(Node<T>* node, T key) {
        if (!node || node->key == key) return node;
        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    void inorder(Node<T>* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    void preorder(Node<T>* node) {
        if (!node) return;
        cout << node->key << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(Node<T>* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->key << " ";
    }

    void bfs(Node<T>* node) {
        if (!node) return;
        queue<Node<T>*> q;
        q.push(node);
        while (!q.empty()) {
            Node<T>* curr = q.front();
            q.pop();
            cout << curr->key << " ";
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
    }

    int height(Node<T>* node) {
        if (!node) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    bool isBalanced(Node<T>* node) {
        if (!node) return true;
        int lh = height(node->left);
        int rh = height(node->right);
        return abs(lh - rh) <= 1 && isBalanced(node->left) && isBalanced(node->right);
    }

    void clear(Node<T>* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    Node<T>* minValueNode(Node<T>* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    Node<T>* maxValueNode(Node<T>* node) {
        while (node && node->right) node = node->right;
        return node;
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        clear(root);
    }

    void insert(T key) {
        root = insert(root, key);
    }

    void deleteNode(T key) {
        root = deleteNode(root, key);
    }

    bool search(T key) {
        return search(root, key) != nullptr;
    }

    void inorder() {
        inorder(root);
        cout << endl;
    }

    void preorder() {
        preorder(root);
        cout << endl;
    }

    void postorder() {
        postorder(root);
        cout << endl;
    }

    void bfs() {
        bfs(root);
        cout << endl;
    }

    int height() {
        return height(root);
    }

    bool isBalanced() {
        return isBalanced(root);
    }

    T minValue() {
        Node<T>* node = minValueNode(root);
        return node ? node->key : T();
    }

    T maxValue() {
        Node<T>* node = maxValueNode(root);
        return node ? node->key : T();
    }

    void clear() {
        clear(root);
        root = nullptr;
    }
};

int main() {
    BST<string> tree;

    tree.insert("apple");
    tree.insert("banana");
    tree.insert("cherry");
    tree.insert("date");
    tree.insert("elderberry");
    tree.insert("fig");
    tree.insert("grape");

    cout << "Inorder Traversal: ";
    tree.inorder();

    cout << "Preorder Traversal: ";
    tree.preorder();

    cout << "Postorder Traversal: ";
    tree.postorder();

    cout << "Breadth-First Search (BFS): ";
    tree.bfs();

    cout << "Height of Tree: " << tree.height() << endl;

    cout << "Minimum Value: " << tree.minValue() << endl;
    cout << "Maximum Value: " << tree.maxValue() << endl;

    cout << "Is Balanced: " << (tree.isBalanced() ? "Yes" : "No") << endl;

    cout << "Search 'cherry': " << (tree.search("cherry") ? "Found" : "Not Found") << endl;
    cout << "Search 'kiwi': " << (tree.search("kiwi") ? "Found" : "Not Found") << endl;

    tree.deleteNode("cherry");
    cout << "Inorder Traversal after deleting 'cherry': ";
    tree.inorder();

    tree.clear();
    cout << "Tree cleared." << endl;
    cout << "Inorder Traversal after clearing: ";
    tree.inorder();

    return 0;
}
