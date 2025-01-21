#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList() : head(nullptr) {}

    T front() {
        if (head == nullptr) {
            cout << "La lista esta vacia" << endl;
            return T();
        } else {
            return head->data;
        }
    }

    T back() {
        if (head == nullptr) {
            cout << "Lista vacia" << endl;
            return T();
        } else {
            Node<T>* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            return temp->data;
        }
    }

    void push_front(T valor) {
        Node<T>* nodo = new Node<T>;
        nodo->data = valor;
        nodo->next = head;
        head = nodo;
    }

    void push_back(T valor) {
        Node<T>* nodo = new Node<T>;
        nodo->data = valor;
        nodo->next = nullptr;
        Node<T>* temp = head;
        if (head == nullptr) {
            head = nodo;
        } else {
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = nodo;
        }
    }

    T pop_front() {
        if (head == nullptr) {
            cout << "No se puede eliminar, la lista vacia" << endl;
            return T();
        } else {
            Node<T>* temp = head;
            T valor = temp->data;
            head = head->next;
            delete temp;
            return valor;
        }
    }

    T pop_back() {
        if (head == nullptr) {
            cout << "Lista vacia" << endl;
            return T();
        } else if (head->next == nullptr) {
            T valor = head->data;
            delete head;
            head = nullptr;
            return valor;
        } else {
            Node<T>* temp = head;
            while (temp->next->next != nullptr)
                temp = temp->next;
            T valor = temp->next->data;
            delete temp->next;
            temp->next = nullptr;
            return valor;
        }
    }

    T operator[](int valor) {
        Node<T>* temp = head;
        for (int i = 0; i < valor; i++) {
            if (temp == nullptr) break;
            temp = temp->next;
        }
        return temp != nullptr ? temp->data : T();
    }

    bool empty() {
        return head == nullptr;
    }

    int size() {
        int s = 0;
        Node<T>* temp = head;
        while (temp != nullptr) {
            s++;
            temp = temp->next;
        }
        return s;
    }

    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* current = head;
        Node<T>* next = nullptr;
        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }

    void sort() { //con bubble sort
        if (head != nullptr) {
            bool swap;
            Node<T>* temp;
            Node<T>* ultimo = nullptr;
            do {
                swap = false;
                temp = head;
                while (temp->next != ultimo) {
                    if (temp->data > temp->next->data) {
                        T t = temp->data;
                        temp->data = temp->next->data;
                        temp->next->data = t;
                        swap = true;
                    }
                    temp = temp->next;
                }
                ultimo = temp;
            } while (swap);
        }
    }

    void print() {
        Node<T>* temp = head;
        while (temp != nullptr) {
            cout << temp->data;
            if (temp->next != nullptr) {
                cout << " -> ";
            }
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    LinkedList<int> lista;
    int op, val, indice;

    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Agregar al frente\n";
        cout << "2. Agregar al final\n";
        cout << "3. Eliminar del frente\n";
        cout << "4. Eliminar del final\n";
        cout << "5. Ver primer elemento\n";
        cout << "6. Ver Ultimo elemento\n";
        cout << "7. Verificar si esta vacia\n";
        cout << "8. Obtener tamanio\n";
        cout << "9. Obtener elemento por indice\n";
        cout << "10. Revertir lista\n";
        cout << "11. Ordenar lista\n";
        cout << "12. Limpiar lista\n";
        cout << "0. Salir\n";
        cout << "Selecciona una opcion: ";
        cin >> op;

        switch (op) {
            case 1:
                cout << "Ingresa un valor: ";
                cin >> val;
                lista.push_front(val);
                lista.print();
                break;
            case 2:
                cout << "Ingresa un valor: ";
                cin >> val;
                lista.push_back(val);
                lista.print();
                break;
            case 3:
                cout << "Elemento eliminado del frente: " << lista.pop_front() << endl;
                lista.print();
                break;
            case 4:
                cout << "Elemento eliminado del final: " << lista.pop_back() << endl;
                lista.print();
                break;
            case 5:
                cout << "Primer elemento: " << lista.front() << endl;
                break;
            case 6:
                cout << "Ultimo elemento: " << lista.back() << endl;
                break;
            case 7:
                cout << (lista.empty() ? "La lista esta vacia" : "La lista no esta vacia") << endl;
                break;
            case 8:
                cout << "Tamanio de la lista: " << lista.size() << endl;
                break;
            case 9:
                cout << "Ingresa el indice (comienza desde 0): ";
                cin >> indice;
                if (indice >= 0 && indice < lista.size())
                    cout << "Elemento en indice " << indice << ": " << lista[indice] << endl;
                else
                    cout << "indice invalido" << endl;
                break;
            case 10:
                lista.reverse();
                cout << "Lista reversed." << endl;
                lista.print();
                break;
            case 11:
                lista.sort();
                cout << "Lists ordenada." << endl;
                lista.print();
                break;
            case 12:
                lista.clear();
                cout << "List limpia." << endl;
                lista.print();
                break;
            case 0:
                break;
            default:
                cout << "Opción inválida, intenta nuevamente." << endl;
        }
    } while (op != 0);

    return 0;
}
