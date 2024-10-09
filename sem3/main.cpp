#include <iostream>
using namespace std;

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Retorna el elemento al comienzo
    T front() {
        if (!empty()) {
            return head->data;
        }
        throw out_of_range("List is empty");
    }

    // Retorna el elemento al final
    T back() {
        if (!empty()) {
            return tail->data;
        }
        throw out_of_range("List is empty");
    }

    // Agrega un elemento al comienzo
    void push_front(T value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++count;
    }

    // Agrega un elemento al final
    void push_back(T value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        ++count;
    }

    // Remueve el elemento al comienzo
    T pop_front() {
        if (empty()) {
            throw out_of_range("List is empty");
        }
        T value = head->data;
        Node* temp = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        --count;
        return value;
    }

    // Remueve el elemento al final
    T pop_back() {
        if (empty()) {
            throw out_of_range("List is empty");
        }
        T value = tail->data;
        Node* temp = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        --count;
        return value;
    }

    // Inserta en cualquier posición
    void insert(T value, int index) {
        if (index < 0 || index > count) {
            throw out_of_range("Index out of range");
        }
        if (index == 0) {
            push_front(value);
        } else if (index == count) {
            push_back(value);
        } else {
            Node* newNode = new Node(value);
            Node* current = head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
            ++count;
        }
    }

    // Remueve en cualquier posición
    void remove(int index) {
        if (index < 0 || index >= count) {
            throw out_of_range("Index out of range");
        }
        if (index == 0) {
            pop_front();
        } else if (index == count - 1) {
            pop_back();
        } else {
            Node* current = head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --count;
        }
    }

    // Retorna el elemento en la posición indicada
    T operator[](int index) {
        if (index < 0 || index >= count) {
            throw out_of_range("Index out of range");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    // Retorna si la lista está vacía o no
    bool empty() {
        return count == 0;
    }

    // Retorna el tamaño de la lista
    int size() {
        return count;
    }

    // Elimina todos los elementos de la lista
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // Reversa la lista
    void reverse() {
        if (empty() || count == 1) return;
        Node* current = head;
        Node* temp = nullptr;
        tail = head;
        while (current != nullptr) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        if (temp != nullptr) {
            head = temp->prev;
        }
    }

    // Destructor para liberar la memoria
    ~DoublyLinkedList() {
        clear();
    }
};

///EJERCICIO1
template <typename T>
bool isPalindrome(DoublyLinkedList<T>& list) {
    if (list.empty()) return true;

    typename DoublyLinkedList<T>::Node* left = list.head;
    typename DoublyLinkedList<T>::Node* right = list.tail;

    while (left != right && left->prev != right) {
        if (left->data != right->data) {
            return false;
        }
        left = left->next;
        right = right->prev;
    }
    return true;
}

///EJERCICIO2
template <typename T>
void SortedInsert(DoublyLinkedList<T>& list, T value) {
    if (list.empty() || list.front() >= value) {
        list.push_front(value);
        return;
    }

    typename DoublyLinkedList<T>::Node* current = list.head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }

    if (current == list.tail) {
        list.push_back(value);
    } else {
        list.insert(value, list.indexOf(current->next->data));  // Función `indexOf` es un ejemplo para obtener el índice
    }
}

///EJERCICIO3
template <typename T>
void removeDuplicates(DoublyLinkedList<T>& list) {
    if (list.size() < 2) return;

    typename DoublyLinkedList<T>::Node* current = list.head;
    while (current != nullptr) {
        typename DoublyLinkedList<T>::Node* runner = current->next;
        while (runner != nullptr) {
            if (current->data == runner->data) {
                typename DoublyLinkedList<T>::Node* duplicate = runner;
                runner = runner->next;
                list.removeNode(duplicate);  // Función `removeNode` que elimina un nodo específico
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}

///EJERCICIO4
template <typename T>
DoublyLinkedList<T> unionOfSets(DoublyLinkedList<T>& list1, DoublyLinkedList<T>& list2) {
    DoublyLinkedList<T> result;

    typename DoublyLinkedList<T>::Node* current1 = list1.head;
    typename DoublyLinkedList<T>::Node* current2 = list2.head;

    while (current1 != nullptr && current2 != nullptr) {
        if (current1->data < current2->data) {
            result.push_back(current1->data);
            current1 = current1->next;
        } else if (current1->data > current2->data) {
            result.push_back(current2->data);
            current2 = current2->next;
        } else {
            result.push_back(current1->data);
            current1 = current1->next;
            current2 = current2->next;
        }
    }

    // Añadir los elementos restantes
    while (current1 != nullptr) {
        result.push_back(current1->data);
        current1 = current1->next;
    }

    while (current2 != nullptr) {
        result.push_back(current2->data);
        current2 = current2->next;
    }

    return result;
}


int main() {
    DoublyLinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    cout << "Front: " << list.front() << endl; // 10
    cout << "Back: " << list.back() << endl; // 30

    list.push_front(5);
    cout << "New Front: " << list.front() << endl; // 5

    list.insert(15, 2);
    cout << "Element at index 2: " << list[2] << endl; // 15

    list.remove(2);
    cout << "Element at index 2 after removal: " << list[2] << endl; // 20

    list.reverse();
    cout << "Front after reverse: " << list.front() << endl; // 30
    cout << "Back after reverse: " << list.back() << endl; // 5

    return 0;
}




