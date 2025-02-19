#include <iostream>
#include <vector>
using namespace std;

template <class T>
class Node {
public:
    int pos_row;
    int pos_col;
    Node<T>* next_row;
    Node<T>* next_col;
    Node(int row, int col) : pos_row(row), pos_col(col), next_row(nullptr), next_col(nullptr) {}
};

template <class T>
class ElementNode : public Node<T> {
public:
    T data;
    ElementNode(int row, int col, T value) : Node<T>(row, col), data(value) {}
};

template <class T>
class HeaderNode : public Node<T> {
public:
    HeaderNode(int row, int col) : Node<T>(row, col) {}
};

template <class T>
class SparseMatrix {
private:
    int n_rows;
    int n_cols;
    vector<HeaderNode<T>*> row_headers;
    vector<HeaderNode<T>*> col_headers;

public:
    SparseMatrix(int rows, int cols) : n_rows(rows), n_cols(cols) {
        row_headers.resize(rows, nullptr);
        col_headers.resize(cols, nullptr);

        for (int i = 0; i < rows; ++i) {
            row_headers[i] = new HeaderNode<T>(i, -1);
        }
        for (int j = 0; j < cols; ++j) {
            col_headers[j] = new HeaderNode<T>(-1, j);
        }
    }

    ~SparseMatrix() {
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current = row_headers[i];
            while (current != nullptr) {
                Node<T>* temp = current;
                current = current->next_row;
                delete temp;
            }
        }
        for (int j = 0; j < n_cols; ++j) {
            Node<T>* current = col_headers[j];
            while (current != nullptr) {
                Node<T>* temp = current;
                current = current->next_col;
                delete temp;
            }
        }
    }

    void insert(int row, int col, T value) {
        if (row < 0 || row >= n_rows || col < 0 || col >= n_cols) {
            cout << "Invalid position!" << endl;
            return;
        }

        ElementNode<T>* new_element = new ElementNode<T>(row, col, value);

        // Insert into row
        Node<T>* current_row = row_headers[row];
        while (current_row->next_row != nullptr && current_row->next_row->pos_col < col) {
            current_row = current_row->next_row;
        }
        new_element->next_row = current_row->next_row;
        current_row->next_row = new_element;

        // Insert into column
        Node<T>* current_col = col_headers[col];
        while (current_col->next_col != nullptr && current_col->next_col->pos_row < row) {
            current_col = current_col->next_col;
        }
        new_element->next_col = current_col->next_col;
        current_col->next_col = new_element;
    }

    // Overload operator () to get value at (i, j)
    T operator()(int row, int col) {
        if (row < 0 || row >= n_rows || col < 0 || col >= n_cols) {
            cout << "Invalid position!" << endl;
            return T();
        }

        Node<T>* current = row_headers[row]->next_row;
        while (current != nullptr && current->pos_col <= col) {
            if (current->pos_col == col) {
                return static_cast<ElementNode<T>*>(current)->data;
            }
            current = current->next_row;
        }
        return T(); // Return default value if not found
    }

// Compare two matrices
    bool operator==(const SparseMatrix<T>& other) {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            return false;
        }

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = row_headers[i]->next_row;
            Node<T>* current2 = other.row_headers[i]->next_row;

            while (current1 != nullptr && current2 != nullptr) {
                if (current1->pos_col != current2->pos_col ||
                    static_cast<ElementNode<T>*>(current1)->data != static_cast<ElementNode<T>*>(current2)->data) {
                    return false;
                }
                current1 = current1->next_row;
                current2 = current2->next_row;
            }

            // If one list has more elements than the other
            if (current1 != nullptr || current2 != nullptr) {
                return false;
            }
        }
        return true;
    }

// Add two matrices
    SparseMatrix<T> operator+(const SparseMatrix<T>& other) {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            throw invalid_argument("Matrices must have the same dimensions for addition.");
        }

        SparseMatrix<T> result(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = row_headers[i]->next_row;
            Node<T>* current2 = other.row_headers[i]->next_row;

            while (current1 != nullptr && current2 != nullptr) {
                if (current1->pos_col == current2->pos_col) {
                    T sum = static_cast<ElementNode<T>*>(current1)->data + static_cast<ElementNode<T>*>(current2)->data;
                    result.insert(i, current1->pos_col, sum);
                    current1 = current1->next_row;
                    current2 = current2->next_row;
                } else if (current1->pos_col < current2->pos_col) {
                    result.insert(i, current1->pos_col, static_cast<ElementNode<T>*>(current1)->data);
                    current1 = current1->next_row;
                } else {
                    result.insert(i, current2->pos_col, static_cast<ElementNode<T>*>(current2)->data);
                    current2 = current2->next_row;
                }
            }

            while (current1 != nullptr) {
                result.insert(i, current1->pos_col, static_cast<ElementNode<T>*>(current1)->data);
                current1 = current1->next_row;
            }

            while (current2 != nullptr) {
                result.insert(i, current2->pos_col, static_cast<ElementNode<T>*>(current2)->data);
                current2 = current2->next_row;
            }
        }
        return result;
    }

// Subtract two matrices
    SparseMatrix<T> operator-(const SparseMatrix<T>& other) {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            throw invalid_argument("Matrices must have the same dimensions for subtraction.");
        }

        SparseMatrix<T> result(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = row_headers[i]->next_row;
            Node<T>* current2 = other.row_headers[i]->next_row;

            while (current1 != nullptr && current2 != nullptr) {
                if (current1->pos_col == current2->pos_col) {
                    T diff = static_cast<ElementNode<T>*>(current1)->data - static_cast<ElementNode<T>*>(current2)->data;
                    result.insert(i, current1->pos_col, diff);
                    current1 = current1->next_row;
                    current2 = current2->next_row;
                } else if (current1->pos_col < current2->pos_col) {
                    result.insert(i, current1->pos_col, static_cast<ElementNode<T>*>(current1)->data);
                    current1 = current1->next_row;
                } else {
                    result.insert(i, current2->pos_col, -static_cast<ElementNode<T>*>(current2)->data);
                    current2 = current2->next_row;
                }
            }

            while (current1 != nullptr) {
                result.insert(i, current1->pos_col, static_cast<ElementNode<T>*>(current1)->data);
                current1 = current1->next_row;
            }

            while (current2 != nullptr) {
                result.insert(i, current2->pos_col, -static_cast<ElementNode<T>*>(current2)->data);
                current2 = current2->next_row;
            }
        }
        return result;
    }

// Multiply two matrices
    SparseMatrix<T> operator*(const SparseMatrix<T>& other) {
        if (n_cols != other.n_rows) {
            throw invalid_argument("Number of columns in the first matrix must match the number of rows in the second matrix for multiplication.");
        }

        SparseMatrix<T> result(n_rows, other.n_cols);

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < other.n_cols; ++j) {
                T sum = T();
                Node<T>* current1 = row_headers[i]->next_row;
                Node<T>* current2 = other.col_headers[j]->next_col;

                while (current1 != nullptr && current2 != nullptr) {
                    if (current1->pos_col == current2->pos_row) {
                        sum += static_cast<ElementNode<T>*>(current1)->data * static_cast<ElementNode<T>*>(current2)->data;
                        current1 = current1->next_row;
                        current2 = current2->next_col;
                    } else if (current1->pos_col < current2->pos_row) {
                        current1 = current1->next_row;
                    } else {
                        current2 = current2->next_col;
                    }
                }

                if (sum != T()) {
                    result.insert(i, j, sum);
                }
            }
        }
        return result;
    }

    void print() {
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current = row_headers[i]->next_row;
            for (int j = 0; j < n_cols; ++j) {
                if (current != nullptr && current->pos_col == j) {
                    cout << static_cast<ElementNode<T>*>(current)->data << " ";
                    current = current->next_row;
                } else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }
};

int main() {
    SparseMatrix<int> matrix1(3, 3);
    matrix1.insert(0, 0, 1);
    matrix1.insert(1, 1, 2);
    matrix1.insert(2, 2, 3);

    SparseMatrix<int> matrix2(3, 3);
    matrix2.insert(0, 0, 4);
    matrix2.insert(1, 1, 5);
    matrix2.insert(2, 2, 6);

    cout << "Matrix 1:" << endl;
    matrix1.print();

    cout << "Matrix 2:" << endl;
    matrix2.print();

    cout << "Matrix 1 + Matrix 2:" << endl;
    (matrix1 + matrix2).print();

    cout << "Matrix 1 - Matrix 2:" << endl;
    (matrix1 - matrix2).print();

    cout << "Matrix 1 * Matrix 2:" << endl;
    (matrix1 * matrix2).print();

    cout << "Matrix 1 == Matrix 2: " << (matrix1 == matrix2 ? "True" : "False") << endl;

    return 0;
}