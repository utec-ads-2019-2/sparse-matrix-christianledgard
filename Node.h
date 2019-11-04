#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class SparseMatrix;

template <typename T>
class Node {
protected:
    T data;
    unsigned int posROW, posCOL;
    Node<T> *next, *down;

public:
    Node(){
        next = down = nullptr;
    };

    friend class SparseMatrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
