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
class CircularSparseMatrix {
private:
    int n_rows;
    int n_cols;
    HeaderNode<T>* row_heads;
    HeaderNode<T>* col_heads;

public:
    CircularSparseMatrix(int rows, int cols) : n_rows(rows), n_cols(cols) {
        HeaderNode<T>* current_row = nullptr;
        for (int i = 0; i < rows; ++i) {
            HeaderNode<T>* header = new HeaderNode<T>(i, -1);
            if (!row_heads) {
                row_heads = header;
            } else {
                current_row->next_row = header;
            }
            header->next_row = row_heads; // Circular link
            current_row = header;
        }

        HeaderNode<T>* current_col = nullptr;
        for (int j = 0; j < cols; ++j) {
            HeaderNode<T>* header = new HeaderNode<T>(-1, j);
            if (!col_heads) {
                col_heads = header;
            } else {
                current_col->next_col = header;
            }
            header->next_col = header; // Circular link
            current_col = header;
        }
    }

    ~CircularSparseMatrix() {
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current = row_heads->next_row; // Start from the first real row header
            while (current != row_heads && current != nullptr) { // Check for dummy head and null
                Node<T>* temp = current;
                current = current->next_row;
                delete temp;
            }
            delete row_heads->next_row; // Delete the row headers themselves
        }
        delete row_heads; // Delete the dummy head

        for (int j = 0; j < n_cols; ++j) {
            Node<T>* current = col_heads->next_col; // Start from the first real col header
            while (current != col_heads && current != nullptr) { // Check for dummy head and null
                Node<T>* temp = current;
                current = current->next_col;
                delete temp;
            }
            delete col_heads->next_col; // Delete the col headers
        }
        delete col_heads; // Delete the dummy head

    }

    void insert(int row, int col, T value) {
        if (row < 0 || row >= n_rows || col < 0 || col >= n_cols) {
            cout << "Invalid position!" << endl;
            return;
        }

        ElementNode<T>* new_element = new ElementNode<T>(row, col, value);

        // Insert into row
        Node<T>* current_row = row_heads->next_row; // Start from the first real row header
        for (; current_row != row_heads && current_row->pos_row != row; current_row = current_row->next_row); //find the row header
        while (current_row->next_row != current_row && current_row->next_row->pos_col < col) {
            current_row = current_row->next_row;
        }
        new_element->next_row = current_row->next_row;
        current_row->next_row = new_element;

        // Insert into column
        Node<T>* current_col = col_heads->next_col; // Start from the first real col header
        for (; current_col != col_heads && current_col->pos_col != col; current_col = current_col->next_col); //find the col header
        while (current_col->next_col != current_col && current_col->next_col->pos_row < row) {
            current_col = current_col->next_col;
        }
        new_element->next_col = current_col->next_col;
        current_col->next_col = new_element;
    }

    T operator()(int row, int col) {
        if (row < 0 || row >= n_rows || col < 0 || col >= n_cols) {
            cout << "Invalid position!" << endl;
            return T();
        }

        Node<T>* current = row_heads->next_row;
        for (; current != row_heads && current->pos_row != row; current = current->next_row); //find the row header
        current = current->next_row; // Go to the first element in the row
        while (current != row_heads && current->pos_col <= col) {
            if (current->pos_col == col) {
                return static_cast<ElementNode<T>*>(current)->data;
            }
            current = current->next_row;
        }
        return T(); // Return default value if not found
    }


    // Compare two matrices
    bool operator==(const CircularSparseMatrix<T>& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            return false;
        }

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = row_heads->next_row;
            for (; current1 != row_heads && current1->pos_row != i; current1 = current1->next_row); //find the row header
            current1 = current1->next_row; // Go to the first element in the row

            Node<T>* current2 = other.row_heads->next_row;
            for (; current2 != other.row_heads && current2->pos_row != i; current2 = current2->next_row); //find the row header
            current2 = current2->next_row; // Go to the first element in the row


            while (current1 != row_heads && current2 != other.row_heads) {
                if (current1->pos_col != current2->pos_col ||
                    static_cast<ElementNode<T>*>(current1)->data != static_cast<ElementNode<T>*>(current2)->data) {
                    return false;
                }
                current1 = current1->next_row;
                current2 = current2->next_row;
            }

            if (current1 != row_heads || current2 != other.row_heads) {
                return false;
            }
        }
        return true;
    }

    CircularSparseMatrix<T> operator+(const CircularSparseMatrix<T>& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            throw invalid_argument("Matrices must have the same dimensions for addition.");
        }

        CircularSparseMatrix<T> result(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = row_heads->next_row;
            for (; current1 != row_heads && current1->pos_row != i; current1 = current1->next_row); //find the row header
            current1 = current1->next_row; // Go to the first element in the row

            Node<T>* current2 = other.row_heads->next_row;
            for (; current2 != other.row_heads && current2->pos_row != i; current2 = current2->next_row); //find the row header
            current2 = current2->next_row; // Go to the first element in the row

            while (current1 != row_heads && current2 != other.row_heads) {
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

            while (current1 != row_heads) {
                result.insert(i, current1->pos_col, static_cast<ElementNode<T>*>(current1)->data);
                current1 = current1->next_row;
            }

            while (current2 != other.row_heads) {
                result.insert(i, current2->pos_col, static_cast<ElementNode<T>*>(current2)->data);
                current2 = current2->next_row;
            }
        }
        return result;
    }

    CircularSparseMatrix<T> operator-(const CircularSparseMatrix<T>& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            throw invalid_argument("Matrices must have the same dimensions for subtraction.");
        }

        CircularSparseMatrix<T> result(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = row_heads->next_row;
            for (; current1 != row_heads && current1->pos_row != i; current1 = current1->next_row); //find the row header
            current1 = current1->next_row; // Go to the first element in the row

            Node<T>* current2 = other.row_heads->next_row;
            for (; current2 != other.row_heads && current2->pos_row != i; current2 = current2->next_row); //find the row header
            current2 = current2->next_row; // Go to the first element in the row

            while (current1 != row_heads && current2 != other.row_heads) {
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

            while (current1 != row_heads) {
                result.insert(i, current1->pos_col, static_cast<ElementNode<T>*>(current1)->data);
                current1 = current1->next_row;
            }

            while (current2 != other.row_heads) {
                result.insert(i, current2->pos_col, -static_cast<ElementNode<T>*>(current2)->data);
                current2 = current2->next_row;
            }
        }
        return result;
    }


    CircularSparseMatrix<T> operator*(const CircularSparseMatrix<T>& other) const {
        if (n_cols != other.n_rows) {
            throw invalid_argument("Number of columns in the first matrix must match the number of rows in the second matrix for multiplication.");
        }

        CircularSparseMatrix<T> result(n_rows, other.n_cols);

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < other.n_cols; ++j) {
                T sum = T();

                Node<T>* current1 = row_heads->next_row;
                for (; current1 != row_heads && current1->pos_row != i; current1 = current1->next_row); //find the row header
                current1 = current1->next_row; // Go to the first element in the row

                Node<T>* current2 = other.col_heads->next_col;
                for (; current2 != other.col_heads && current2->pos_col != j; current2 = current2->next_col); //find the col header
                current2 = current2->next_col; // Go to the first element in the col

                while (current1 != row_heads && current2 != other.col_heads) {
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
            Node<T>* current = row_heads->next_row; // Start from the first real row header
            for (; current != row_heads && current->pos_row != i; current = current->next_row); //find the row header
            current = current->next_row; // Go to the first element in the row

            for (int j = 0; j < n_cols; ++j) {
                if (current != row_heads && current->pos_col == j) {
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
    CircularSparseMatrix<int> matrix1(3, 3);
    matrix1.insert(0, 0, 1);
    matrix1.insert(0, 1, 5);
    matrix1.insert(0, 2, 1);
    matrix1.insert(1, 1, 2);
    matrix1.insert(2, 1, 4);
    matrix1.insert(2, 2, 3);

    CircularSparseMatrix<int> matrix2(3, 3);
    matrix2.insert(0, 0, 4);
    matrix2.insert(0, 1, 7);
    matrix2.insert(0, 2, 4);
    matrix2.insert(1, 1, 5);
    matrix2.insert(2, 0, 8);
    matrix2.insert(2, 1, 1);
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