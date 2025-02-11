#include <iostream>
#include <queue>
using namespace std;

template <typename T>
struct Node {
    T val;
    int bf; // Balance factor: -1, 0, 1
    Node* left;
    Node* right;

    Node(T val) : val(val), bf(0), left(nullptr), right(nullptr) {}
};

template <typename T>
class AVL {
private:
    Node<T>* root;

    //LL
    Node<T>* rotateRight(Node<T>* node) {
        Node<T>* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        node->bf = (node->bf == 1 ? 0 : -1);
        newRoot->bf = (newRoot->bf == -1 ? 0 : 1);

        return newRoot;
    }
    //RR
    Node<T>* rotateLeft(Node<T>* node) {
        Node<T>* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;

        node->bf = (node->bf == -1 ? 0 : 1);
        newRoot->bf = (newRoot->bf == 1 ? 0 : -1);

        return newRoot;
    }
    //RL
    Node<T>* rotateLeftRight(Node<T>* node) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    //LR
    Node<T>* rotateRightLeft(Node<T>* node) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    Node<T>* insert(Node<T>* node, T value, bool& heightChanged) {
        if (!node) {
            heightChanged = true;
            return new Node<T>(value);
        }

        if (value < node->val) {
            node->left = insert(node->left, value, heightChanged);
            if (heightChanged) {
                node->bf++;
                if (node->bf == 2) {
                    heightChanged = false;
                    if (value < node->left->val)
                        return rotateRight(node); // LL rotation
                    else
                        return rotateLeftRight(node); // LR rotation
                }
            }
        } else if (value > node->val) {
            node->right = insert(node->right, value, heightChanged);
            if (heightChanged) {
                node->bf--;
                if (node->bf == -2) {
                    heightChanged = false;
                    if (value > node->right->val)
                        return rotateLeft(node); // RR rotation
                    else
                        return rotateRightLeft(node); // RL rotation
                }
            }
        } else {
            heightChanged = false; // Duplicate value are not allowed
        }

        if (node->bf == 0) heightChanged = false;
        return node;
    }

    Node<T>* deleteNode(Node<T>* node, T val, bool& heightChanged) {
        if (!node) {
            heightChanged = false;
            return nullptr;
        }

        if (val < node->val) {
            node->left = deleteNode(node->left, val, heightChanged);
            if (heightChanged) {
                node->bf--;
                if (node->bf == -2) {
                    if (node->right->bf <= 0) {
                        node = rotateLeft(node); // RR rotation
                        heightChanged = node->bf != 0;
                    } else {
                        node = rotateRightLeft(node); // RL rotation
                        heightChanged = true;
                    }
                } else {
                    heightChanged = node->bf != 0;
                }
            }
        } else if (val > node->val) {
            node->right = deleteNode(node->right, val, heightChanged);
            if (heightChanged) {
                node->bf++;
                if (node->bf == 2) {
                    if (node->left->bf >= 0) {
                        node = rotateRight(node); // LL rotation
                        heightChanged = node->bf != 0;
                    } else {
                        node = rotateLeftRight(node); // LR rotation
                        heightChanged = true;
                    }
                } else {
                    heightChanged = node->bf != 0;
                }
            }
        } else {
            // Node with one or no child
            if (!node->left || !node->right) {
                Node<T>* temp = node->left ? node->left : node->right;
                delete node;
                heightChanged = true;
                return temp;
            }

            // Node with two children
            Node<T>* temp = findSuccessor(node->right);
            node->val = temp->val;
            node->right = deleteNode(node->right, temp->val, heightChanged);
            if (heightChanged) {
                node->bf++;
                if (node->bf == 2) {
                    if (node->left->bf >= 0) {
                        node = rotateRight(node); // LL rotation
                        heightChanged = node->bf != 0;
                    } else {
                        node = rotateLeftRight(node); // LR rotation
                        heightChanged = true;
                    }
                } else {
                    heightChanged = node->bf != 0;
                }
            }
        }
        return node;
    }

    Node<T>* findPredecessor(Node<T>* node) {
        while (node && node->right) node = node->right;
        return node;
    }

    Node<T>* findSuccessor(Node<T>* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    void clear(Node<T>* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    Node<T>* minValueNode(Node<T>* node) {
        Node<T>* current = node;
        while (current->left != nullptr) {current = current->left;}
        return current;
    }

    void preorder(Node<T>* root, vector<T>& ret) {
        if (root != nullptr) {
            ret.push_back(root->data);
            preorder(root->left, ret);
            preorder(root->right, ret);
        }
    }

    void inorder(Node<T>* root, vector<T>& ret) {
        if (root != nullptr) {
            inorder(root->left, ret);
            ret.push_back(root->data);
            inorder(root->right, ret);
        }
    }

    void postorder(Node<T>* root, vector<T>& ret) {
        if (root != nullptr) {
            postorder(root->left, ret);
            postorder(root->right, ret);
            ret.push_back(root->data);
        }
    }

public:
    AVL() : root(nullptr) {}

    ~AVL() {
        clear(root);
    }

    void insert(T val) {
        bool heightChanged = false;
        root = insert(root, val, heightChanged);
    }

    void deleteNode(T val) {
        bool heightChanged = false;
        root = deleteNode(root, val, heightChanged);
    }

    bool search(T val) {
        Node<T>* current = root;
        while (current) {
            if (val == current->val) return true;
            current = val < current->val ? current->left : current->right;
        }
        return false;
    }

    void bfs() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        queue<Node<T>*> q;
        q.push(root);

        while (!q.empty()) {
            Node<T>* node = q.front();
            q.pop();
            cout << node->val << " ";
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        cout << endl;
    }
};

int main() {
    AVL<int> tree;

    // Insert elements
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(8);

    cout << "BFS traversal: ";
    tree.bfs();

    // Delete an element
    tree.deleteNode(10);
    cout << "BFS traversal after deleting 10: ";
    tree.bfs();

    // Search for an element
    cout << "Search for 6: " << (tree.search(6) ? "Found" : "Not Found") << endl;

    return 0;
}
