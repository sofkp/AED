#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* next;

    Node(int val) : data(val), next(nullptr) {}
};

class Stack {
private:
    Node* topNode;
    int size;
    int capacity;

public:
    Stack(int cap = INT_MAX) : topNode(nullptr), size(0), capacity(cap) {}

    void push(int val) {
        if (isFull()) {
            cout << "Stack is full. Cannot push " << val << "." << endl;
            return;
        }
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
        size++;
    }

    int pop() {
        if (isEmpty()) {
            cout << "Stack is empty. Cannot pop." << endl;
            return -1;
        }
        Node* temp = topNode;
        int poppedValue = temp->data;
        topNode = topNode->next;
        delete temp;
        size--;
        return poppedValue;
    }

    int top() {
        if (isEmpty()) {
            cout << "Stack is empty. No top element." << endl;
            return -1;
        }
        return topNode->data;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size >= capacity;
    }

    void printStack() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return;
        }
        cout << "Stack: ";
        Node* temp = topNode;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

int main() {
    Stack s(5);

    cout << "Pushing elements onto the stack..." << endl;
    cout << " - push 10 " << endl;
    s.push(10);
    cout << " - push 20 " << endl;
    s.push(20);
    cout << " - push 30 " << endl;
    s.push(30);
    cout << " - push 40 " << endl;
    s.push(40);
    cout << " - push 50 " << endl;
    s.push(50);
    cout << " - push 60 " << endl;
    s.push(60);

    s.printStack();

    cout << "Top element: " << s.top() << endl;

    cout << "Popping elements from the stack..." << endl;
    cout << "Popped: " << s.pop() << endl;
    cout << "Popped: " << s.pop() << endl;

    s.printStack();

    cout << "Is the stack empty? " << (s.isEmpty() ? "Yes" : "No") << endl;

    cout << "Pushing elements onto the stack..." << endl;
    cout << " - push 60 " << endl;
    s.push(60);
    cout << " - push 70 " << endl;
    s.push(70);

    s.printStack();

    cout << "Is the stack full? " << (s.isFull() ? "Yes" : "No") << endl;

    return 0;
}
