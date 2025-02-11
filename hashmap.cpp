#include <iostream>
#include <vector>
#include <random>
using namespace std;

struct Node {
    string key;
    int value;
    Node* next;
    Node(string k, int v) : key(k), value(v), next(nullptr) {}
};

class HashMap {
private:
    static const int TABLE_SIZE = 10; // Size of the hash table
    Node* table[TABLE_SIZE];
    int a, b, p;

    void initializeHash() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    int hashFunction(const string& key) const {
        int hash = 0;
        for (char ch : key) {
            hash = (hash * 31 + ch) % p;
        }
        return ((a * hash + b) % p) % TABLE_SIZE;
    }

public:
    HashMap() : p(101) {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
        initializeHash();
    }

    ~HashMap() {
        clear();
    }

    void insert(const string& key, int value) {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    int& operator[](const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        insert(key, 0);
        return table[index]->value;
    }

    int at(const string& key) const {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return 0;
    }

    bool contains(const string& key) const {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void erase(const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }

    bool empty() const {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i]) return false;
        }
        return true;
    }

    size_t size() const {
        size_t count = 0;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current) {
                count++;
                current = current->next;
            }
        }
        return count;
    }

    size_t bucket_count() const {
        return TABLE_SIZE;
    }

    size_t bucket(const string& key) const {
        return hashFunction(key);
    }

    size_t bucket_size(size_t index) const {
        size_t count = 0;
        Node* current = table[index];
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    size_t count(const string& key) const {
        return contains(key) ? 1 : 0;
    }

    void iterate() const {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current) {
                cout << "Key: " << current->key << " Value: " << current->value << endl;
                current = current->next;
            }
        }
    }
};

int main() {
    HashMap map;
    map.insert("clave1", 10);
    map.insert("clave2", 20);
    map.insert("clave3", 30);

    cout << "clave1: " << map.at("clave1") << endl;
    cout << "clave2: " << map["clave2"] << endl;

    map.erase("clave1");

    // Accessing a non-existent key returns the default value (0)
    cout << "clave1: " << map.at("clave1") << endl;

    cout << "Bucket count: " << map.bucket_count() << endl;
    cout << "Is empty: " << map.empty() << endl;
    cout << "Size: " << map.size() << endl;

    cout << "Iterating over hashmap:" << endl;
    map.iterate();

    return 0;
}