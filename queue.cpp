#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class Queue {
private:
    Node* front;
    Node* rear;
    int size;
    int capacity;

public:
    Queue(int cap = INT_MAX) : front(nullptr), rear(nullptr), size(0), capacity(cap) {}

    void enqueue(int val) {
        if (isFull()) {
            cout << "Queue is full. Cannot enqueue " << val << "." << endl;
            return;
        }
        Node* newNode = new Node(val);
        if (rear == nullptr) { // Empty queue
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue." << endl;
            return -1;
        }
        Node* temp = front;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        int data = temp->data;
        delete temp;
        size--;
        return data;
    }

    int first() {
        if (isEmpty()) {
            cout << "Queue is empty. No front element." << endl;
            return -1;
        }
        return front->data;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size >= capacity;
    }

    void printQueue() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }
        Node* temp = front;
        cout << "Queue: ";
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

int main() {
    Queue q(5);

    cout << "Adding elements to the queue..." << endl;
    cout << " - add 10" << endl;
    q.enqueue(10);
    cout << " - add 20" << endl;
    q.enqueue(20);
    cout << " - add 30" << endl;
    q.enqueue(30);
    cout << " - add 40" << endl;
    q.enqueue(40);
    cout << " - add 50" << endl;
    q.enqueue(50);
    cout << " - add 60" << endl;
    q.enqueue(60);

    q.printQueue();

    cout << "First element: " << q.first() << endl;

    cout << "Removing elements from the queue..." << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;

    q.printQueue();

    cout << "Is the queue empty? " << (q.isEmpty() ? "Yes" : "No") << endl;

    cout << "Adding elements to the queue..." << endl;
    cout << " - add 60" << endl;
    q.enqueue(60);
    cout << " - add 70" << endl;
    q.enqueue(70);

    q.printQueue();

    cout << "Is the queue full? " << (q.isFull() ? "Yes" : "No") << endl;

    return 0;
}
