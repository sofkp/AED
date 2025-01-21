#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
};

template <typename T>
class DLList {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    DLList() : head(NULL), tail(NULL) {}

    T front() {
        return head ? head->data : T();
    }

    T back() {
        return tail ? tail->data : T();
    }

    void push_front(T val) {
        Node<T>* n = new Node<T>{val, head, NULL};
        if (head) head->prev = n;
        if (!tail) tail = n;
        head = n;
    }

    void push_back(T val) {
        Node<T>* n = new Node<T>{val, NULL, tail};
        if (tail) tail->next = n;
        if (!head) head = n;
        tail = n;
    }

    T pop_front() {
        if (!head) return T();
        Node<T>* temp = head;
        T val = temp->data;
        head = head->next;
        if (head) head->prev = NULL;
        else tail = NULL;
        delete temp;
        return val;
    }

    T pop_back() {
        if (!tail) return T();
        Node<T>* temp = tail;
        T val = temp->data;
        tail = tail->prev;
        if (tail) tail->next = NULL;
        else head = NULL;
        delete temp;
        return val;
    }

    T operator[](int idx) {
        Node<T>* temp = head;
        for (int i = 0; i < idx; ++i) {
            if (!temp) return T();
            temp = temp->next;
        }
        return temp ? temp->data : T();
    }

    bool empty() {
        return head == NULL;
    }

    int size() {
        int count = 0;
        Node<T>* temp = head;
        while (temp) {
            ++count;
            temp = temp->next;
        }
        return count;
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = NULL;
    }

    void reverse() {
        Node<T>* temp = head;
        head = tail;
        tail = temp;

        while (temp) {
            Node<T>* nxt = temp->next;
            temp->next = temp->prev;
            temp->prev = nxt;
            temp = temp->prev;
        }
    }

    void insert(T val, int pos) {
        if (pos == 0) {
            push_front(val);
            return;
        }
        Node<T>* temp = head;
        for (int i = 0; temp && i < pos - 1; ++i) {
            temp = temp->next;
        }
        if (!temp || temp == tail) {
            push_back(val);
        } else {
            Node<T>* n = new Node<T>{val, temp->next, temp};
            temp->next->prev = n;
            temp->next = n;
        }
    }

    void remove(int pos) {
        if (!head) return;
        Node<T>* temp = head;
        for (int i = 0; temp && i < pos; ++i) {
            temp = temp->next;
        }
        if (!temp) return;

        if (temp == head) head = temp->next;
        if (temp == tail) tail = temp->prev;
        if (temp->prev) temp->prev->next = temp->next;
        if (temp->next) temp->next->prev = temp->prev;

        delete temp;
    }

    void sort() {
        if (!head) return;
        bool swapped;
        do {
            swapped = false;
            Node<T>* temp = head;
            while (temp && temp->next) {
                if (temp->data > temp->next->data) {
                    swap(temp->data, temp->next->data);
                    swapped = true;
                }
                temp = temp->next;
            }
        } while (swapped);
    }

    void print() {
        Node<T>* temp = head;
        cout << "[";
        while (temp) {
            cout << temp->data;
            if (temp->next) cout << " -> ";
            temp = temp->next;
        }
        cout << "]" << endl;
    }
};

int main() {
    DLList<int> dll;

    cout << "*** Bienvenido a la Lista Doblemente Enlazada ***" << endl;

    cout << "Inicializando lista" << endl;
    dll.push_back(3);
    dll.push_front(1);
    dll.push_back(4);
    dll.push_front(0);
    dll.insert(2, 2);
    dll.print();
    cout << endl;

    cout << "Agregando un elemento al final" << endl;
    dll.push_back(5);
    dll.print();
    cout << endl;

    cout << "Primer elemento: " << dll.front() << endl;
    cout << endl;

    cout << "Eliminando el primer elemento" << endl;
    dll.pop_front();
    dll.print();
    cout << endl;

    cout << "Ultimo elemento: " << dll.back() << endl;
    cout << endl;

    cout << "Eliminando el ultimo elemento" << endl;
    dll.pop_back();
    dll.print();
    cout << endl;

    cout << "Lista reversed" << endl;
    dll.reverse();
    dll.print();
    cout << endl;

    cout << "Ordenando la lista" << endl;
    dll.sort();
    dll.print();
    cout << endl;

    cout << "Eliminando elemento en posicion 2" << endl;
    dll.remove(2);
    dll.print();
    cout << endl;

    cout << (dll.empty() ? "La lista esta vacia" : "La lista no esta vacia") << endl;
    
    cout << "Tamanio de la lista: " << dll.size() << endl;
    cout << endl;
    
    cout << "Limpiando la lista" << endl;
    dll.clear();
    cout << "Lista despues de limpiar: " << endl;
    dll.print();
    cout << endl;

    cout << "*** Fin del programa ***";
    return 0;
}
