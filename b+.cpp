#include <iostream>
using namespace std;

class Node {
public:
    int *keys;            
    Node **children;
    Node *next;        // puntero al siguiente nodo hoja (si es hoja)
    int num;          // contador para claves en el nodo
    int m;            // grado del nodo
    bool leaf;

    Node(bool leaf, int m): leaf(leaf), num(0), next(nullptr), m(m) {
        keys = new int[m - 1];
        children = new Node*[m];
        for (int i = 0; i < m; i++) {
            children[i] = nullptr;
        }
    };
    ~Node(){
        delete[] keys;
        delete[] children;
    };
};

class B {
public:
    Node *root;
    int m;

    B(int m) : root(nullptr), m(m) {}
    ~B() {
        delete root;
    }

    void splitChild(Node *parent, int index, Node *child){
        Node *newChild = new Node(child->leaf, m);
        int mid = m / 2 - 1;
        newChild->num = mid;

        // copia las keys dsps del mid del nodo hijo al nuevo nodo
        for (int i = 0; i < newChild->num; i++) {
            newChild->keys[i] = child->keys[i + m / 2];
        }

        if (!child->leaf) { //si es hoja se copia los punteros a los hijos
            for (int i = 0; i < m / 2; i++) {
                newChild->children[i] = child->children[i + m / 2];
            }
        }else { // si es hoja se ajustan los punteros del ultimo nivel
            newChild->next = child->next;
            child->next = newChild;
        }

        //update el num de claves en el hijo original
        child->num = mid;

        //se ajusta el padre para que apunte al nuevo node
        for (int i = parent->num; i > index; i--) {
            parent->children[i + 1] = parent->children[i];
        }
        parent->children[index + 1] = newChild;

        //mueve la clave mid al padre
        for (int i = parent->num - 1; i >= index; i--) {
            parent->keys[i + 1] = parent->keys[i];
        }

        parent->keys[index] = child->keys[mid];
        parent->num++;
    }

    void insertNonFull(Node *node, int key){
        int i = node->num - 1;

        if (node->leaf) {
            //shift a la der para hacer espacio en la posicion correcta de la key
            while (i >= 0 && node->keys[i] > key) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->num++;
        } else { //si no es hoja se recorre el nodo para encontrar el hijo para avanzar
            while (i >= 0 && node->keys[i] > key) {
                i--;
            }
            i++;
            //si hijo esta lleno se divide el hijo
            if (node->children[i]->num == m - 1) {
                splitChild(node, i, node->children[i]);
                //verifica en que hijo iria la key
                if (node->keys[i] < key) {
                    i++;
                }
            }
            //se inserta el key
            insertNonFull(node->children[i], key);
        }
    }

    void insert(int key){
        if (root == nullptr) { //raiz vacia
            root = new Node(true, m);
            root->keys[0] = key;
            root->num = 1;
        } else {
            if (root->num == m - 1) {//raiz llena
                //nueva raiz
                Node *newRoot = new Node(false, m);
                newRoot->children[0] = root;
                // se divide la raiz
                splitChild(newRoot, 0, root);
                //recorrer para encontrar la posicion correcta
                int i = 0;
                if (newRoot->keys[0] < key) {
                    i++;
                }
                insertNonFull(newRoot->children[i], key);
                root = newRoot;
            } else { //raiz no llena
                insertNonFull(root, key);
            }
        }
    }

    Node* auxSearch(Node* node, int key){
        if (node == nullptr) return nullptr;

        int i = 0;
        //recorre el nodo para encontrar su posicion
        while (i < node->num && key > node->keys[i]) {
            i++;
        }
        //si el key esta en el nodo
        if (i < node->num && key == node->keys[i]) {
            return node;
        }
        //si se llega a hoja y no se encontro
        if (node->leaf) {
            return nullptr;
        }
        //si se necesita recorrer otro nodo
        return auxSearch(node->children[i], key);
    };

    Node* search(int key) {
        return auxSearch(root, key);
    }

    void removeFromNode(Node *node, int key){
        int index = 0;
        while (index < node->num && node->keys[index] < key) {
            index++;
        }

        if (index < node->num && node->keys[index] == key) { //se encontro el key en un nodo
            if (node->leaf) {// nodo es hoja, se elima la clave de la hoja
                for (int i = index + 1; i < node->num; i++) {
                    node->keys[i - 1] = node->keys[i];
                }
                node->num--;
            } else {//node no es hoja, se reemplaza el key con el previo y se sigue recursivamente
                int prevKey = findPrev(node, index);
                node->keys[index] = prevKey;
                removeFromNode(node->children[index], prevKey);
            }
        } else {//si no esta en el nodo se sigue recorriendo en los hijos
            if (node->leaf) { //si es hoja ent no existe
                return;
            }

            bool isLastChild = (index == node->num);
            //verificamos si hay suficientes keys
            if (node->children[index]->num < m / 2) {
                underflow(node, index);
            }

            if (isLastChild && index > node->num) { //si undeflow y sibling prev tiene
                removeFromNode(node->children[index - 1], key);
            } else { //si underflow y sibling next tiene
                removeFromNode(node->children[index], key);
            }
        }
    }

    int findPrev(Node *node, int index){
        Node *current = node->children[index];
        while (!current->leaf) {
            current = current->children[current->num];
        }
        return current->keys[current->num - 1];
    }

    void underflow(Node *node, int index){
        if (index != 0 && node->children[index - 1]->num > m / 2) {
            Node *child = node->children[index];
            Node *sibling = node->children[index - 1];

            for (int i = child->num - 1; i >= 0; i--) {
                child->keys[i + 1] = child->keys[i];
            }

            if (!child->leaf) {
                for (int i = child->num; i >= 0; i--) {
                    child->children[i + 1] = child->children[i];
                }
            }

            child->keys[0] = node->keys[index - 1];

            if (!child->leaf) {
                child->children[0] = sibling->children[sibling->num];
            }

            node->keys[index - 1] = sibling->keys[sibling->num - 1];

            child->num++;
            sibling->num--;
        } else if (index != node->num && node->children[index + 1]->num > m / 2) {
            Node *child = node->children[index];
            Node *sibling = node->children[index + 1];

            child->keys[child->num] = node->keys[index];

            if (!child->leaf) {
                child->children[child->num + 1] = sibling->children[0];
            }

            node->keys[index] = sibling->keys[0];

            for (int i = 1; i < sibling->num; i++) {
                sibling->keys[i - 1] = sibling->keys[i];
            }

            if (!sibling->leaf) {
                for (int i = 1; i <= sibling->num; i++) {
                    sibling->children[i - 1] = sibling->children[i];
                }
            }

            child->num++;
            sibling->num--;
        } else {
            if (index != node->num) {
                Node *child = node->children[index];
                Node *sibling = node->children[index + 1];

                child->keys[child->num] = node->keys[index];

                for (int i = 0; i < sibling->num; i++) {
                    child->keys[child->num + 1 + i] = sibling->keys[i];
                }

                if (!child->leaf) {
                    for (int i = 0; i <= sibling->num; i++) {
                        child->children[child->num + 1 + i] = sibling->children[i];
                    }
                }

                for (int i = index + 1; i < node->num; i++) {
                    node->keys[i - 1] = node->keys[i];
                }

                for (int i = index + 2; i <= node->num; i++) {
                    node->children[i - 1] = node->children[i];
                }

                child->num += sibling->num + 1;
                node->num--;

                delete sibling;
            } else {
                Node *child = node->children[index];
                Node *sibling = node->children[index - 1];

                sibling->keys[sibling->num] = node->keys[index - 1];

                for (int i = 0; i < child->num; i++) {
                    sibling->keys[sibling->num + 1 + i] = child->keys[i];
                }

                if (!sibling->leaf) {
                    for (int i = 0; i <= child->num; i++) {
                        sibling->children[sibling->num + 1 + i] = child->children[i];
                    }
                }

                for (int i = index; i < node->num; i++) {
                    node->keys[i - 1] = node->keys[i];
                }

                for (int i = index + 1; i <= node->num; i++) {
                    node->children[i - 1] = node->children[i];
                }

                sibling->num += child->num + 1;
                node->num--;

                delete child;
            }
        }
    }

    void remove(int key){
        if (!root) {
            return;
        }
        removeFromNode(root, key);

        //si raiz vacia
        if (root->num == 0) {
            Node *oldRoot = root;
            if (root->leaf) { //si tiene hijos, se vuelve raiz
                root = nullptr;
            } else {
                root = root->children[0];
            }
            delete oldRoot;
        }
    }

    void printTree() {
        if (root == nullptr) {
            cout << "The tree is empty." << endl;
            return;
        }
        printTreeRecursive(root, 0);
    }

    void printTreeRecursive(Node *node, int level) {
        cout << "Level " << level << ": ";
        for (int i = 0; i < node->num; i++) {
            cout << node->keys[i] << " ";
        }
        cout << endl;

        if (!node->leaf) {
            for (int i = 0; i <= node->num; i++) {
                printTreeRecursive(node->children[i], level + 1);
            }
        }
    }

};


int main() {
    B tree(4);
    tree.insert(10);
    tree.printTree();
    cout<<endl;
    tree.insert(20);
    tree.printTree();
    cout<<endl;
    tree.insert(5);
    tree.printTree();
    cout<<endl;
    tree.insert(6);
    tree.printTree();
    cout<<endl;
    tree.insert(12);
    tree.printTree();
    cout<<endl;
    tree.insert(30);
    tree.printTree();
    cout<<endl;
    tree.insert(7);
    tree.printTree();
    cout<<endl;
    tree.insert(17);
    tree.printTree();
    cout<<endl;

    cout << "\nÁrbol B+ tras inserciones:" << endl;
    tree.printTree();

    Node* result = nullptr;

    result = tree.search(6);
    if(result) {cout << " 6 encontrado\n";} else {cout << " 6 no encontrado\n";}

    result = tree.search(20);
    if(result) {cout << " 20 encontrado\n";} else {cout << " 20 no encontrado\n";}

    result = tree.search(90);
    if(result) {cout << " 90 encontrado\n";} else {cout << " 90 no encontrado\n";}

    tree.remove(6);
    cout << "\nÁrbol B+ tras eliminar 6:" << endl;
    tree.printTree();

    return 0;
}
