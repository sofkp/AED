#include <iostream>
using namespace std;

template <typename T>
struct Nodo {
    T data;
    Nodo* next;
    Nodo* prev;
    Nodo(T data) : data(data), next(nullptr), prev(nullptr) {}
};

template <typename T>
class ListaCircular {
private:
    Nodo<T>* sentinel;
    int tam;
public:
    ListaCircular(){
        sentinel = new Nodo<T>(T());
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        tam = 0;
    }

    ~ListaCircular(){
        clear();
        delete sentinel;
    }

    T front() {
        if (empty()) {
            return -1;
        }
        return sentinel->next->data;
    }

    T back() {
        if (empty()) {
            return -1;
        }
        return sentinel->prev->data;
    }

    void push_front(T value) {
        Nodo<T>* temp = new Nodo<T>(value);
        temp->next = sentinel->next;
        temp->prev = sentinel;
        sentinel->next->prev = temp;
        sentinel->next = temp;
        tam++;
    }

    void push_back(T value) {
        Nodo<T>* temp = new Nodo<T>(value);
        temp->next = sentinel;
        temp->prev = sentinel->prev;
        sentinel->prev->next = temp;
        sentinel->prev = temp;
        tam++;
    }

    T pop_front() {
        if (empty()) {
            return -1;
        }
        Nodo<T>* temp = sentinel->next;
        T val = temp->data;
        sentinel->next = temp->next;
        temp->next->prev = sentinel;

        tam--;
        delete temp;
        return val;
    }

    T pop_back() {
        if (empty()) {
            return -1;
        }
        Nodo<T>* temp = sentinel->prev;
        T val = temp->data;

        sentinel->prev = temp->prev;
        temp->prev->next = sentinel;
        tam--;
        delete temp;
        return val;
    }

    void insert(T val, int pos) {
        if (pos < 0 || pos > tam) {
            cout <<"posicion fuera de rango" << endl;
            return;
        }
        if (pos == 0) {
            push_front(val);
            return;
        } else if (pos == tam) {
            push_back(val);
            return;
        } else {
            Nodo<T>* newNodo = new Nodo<T>(val);
            Nodo<T>* curr = sentinel->next;
            for (int i = 0; i < pos; i++) {
                curr = curr->next;
            }
            newNodo->prev = curr->prev;
            newNodo->next = curr;
            curr->prev->next = newNodo;
            curr->prev = newNodo;
            tam++;
        }
    }

    void remove(int pos) {
        if (pos < 0 || pos >= tam) {
            cout << "posicion fuera de rango" << endl;
            return;
        }
        if (pos == 0) {
            pop_front();
            return;
        } else if (pos == tam - 1) {
            pop_back();
            return;
        } else {
            Nodo<T>* curr = sentinel->next;
            for (int i = 0; i < pos; i++) {
                curr = curr->next;
            }
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            delete curr;
            tam--;
        }
    }

    T operator[](int pos) {
        if (pos < 0 || pos >= tam) {
            cout << "posicion fuera de rango" << endl;
            return -1;
        }
        Nodo<T>* curr = sentinel->next;
        for (int i = 0; i < pos; i++) {
            curr = curr->next;
        }
        return curr->data;
    }

    bool empty() const {
        return tam == 0;
    }

    int size() const {
        return tam;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
        tam = 0;
        sentinel->prev = sentinel;
        sentinel->next =  sentinel;
    }

    void reverse() {
        if (empty()) {
            cout << "lista vacia" << endl;
            return;
        }
        Nodo<T>* current = sentinel->next;
        Nodo<T>* prev = sentinel;
        Nodo<T>* next = nullptr;
        do {
            next = current->next;
            current->next = prev;
            current->prev = next;
            prev = current;
            current = next;
        } while (current != sentinel);
        sentinel->next = prev;
        sentinel->prev = prev->prev;
    }

    void print() const {
        if (empty()) {
            cout << "sentinel" << endl;
            return;
        }
        Nodo<T>* temp = sentinel->next;
        cout << "sentinel -> ";
        do {
            cout << temp->data << " -> ";
            temp = temp->next;
        } while (temp != sentinel);
        cout << "sentinel" << endl;
    }
};

int main() {
    ListaCircular<int> lista;

    lista.push_front(10);
    lista.push_front(20);
    lista.push_front(30);
    cout << "Lista push_front 10, 20 y 30: "; lista.print();
    cout << "El primer elemento es: " << lista.front() << endl;

    lista.push_back(40);
    lista.push_back(50);
    cout << "Lista push_back valores 40 y 50): "; lista.print();
    cout << "El ultimo elemento es: " << lista.back() << endl;

    cout << "Eliminando el primer elemento: " << lista.pop_front() << endl;
    cout << "Lista tras pop_front(): "; lista.print();

    cout << "El nuevo primer elemento es: " << lista.front() << endl;

    cout << "Eliminando el ultimo elemento: " << lista.pop_back() << endl;
    cout << "Lista tras pop_back(): "; lista.print();

    cout << "El nuevo ultimo elemento es: " << lista.back() << endl;

    lista.insert(25, 1);
    cout << "Lista insert(25, 1): "; lista.print();

    cout << "Elemento en posicion 1: " << lista[1] << endl;

    lista.remove(1);
    cout << "Lista tras remove(1): "; lista.print();

    cout << "Elemento en posicion 1: " << lista[1] << endl;

    cout << "La lista esta vacia?: " << (lista.empty() ? "Si" : "No") << endl;

    cout << "El tamanio de la lista es: " << lista.size() << endl;

    lista.clear();

    cout << "Lista tras clear(): "; lista.print();

    cout << "La lista esta vacia despues de clear?: " << (lista.empty() ? "Si" : "No") << endl;

    lista.push_front(60);
    lista.push_front(70);
    lista.push_front(80);

    cout << "Lista push_front 60, 70 y 80: "; lista.print();

    lista.reverse();

    cout << "Lista reverse(): "; lista.print();

    cout << "Primer elemento despues de revertir: " << lista.front() << endl;

    cout << "Ultimo elemento despues de revertir: " << lista.back() << endl;

    return 0;
}
