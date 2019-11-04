#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include "Node.h"

using namespace std;

template <typename T>
class SparseMatrix {
private:
    Node<T> *x;
    Node<T> *y;
    unsigned int rows, columns;

    vector <Node<T>*> R;
    vector <Node<T>*> C;



public:
    SparseMatrix(unsigned rows, unsigned columns);

    void set(unsigned int rowPos, unsigned int colPos, T data);
    T get(unsigned int rowPos, unsigned int colPos);
//    T operator()(unsigned int, unsigned int) const;
//    SparseMatrix<T> operator*(T scalar) const;
//    SparseMatrix<T> operator*(SparseMatrix<T> other) const;
//    SparseMatrix<T> operator+(SparseMatrix<T> other) const;
//    SparseMatrix<T> operator-(SparseMatrix<T> other) const;
//    SparseMatrix<T> transpose() const;
//    void print() const;
//
//    ~SparseMatrix();
};


template<typename T>
SparseMatrix<T>::SparseMatrix(unsigned int rows, unsigned int columns) {
    for (unsigned int i = 0; i < rows + 1; ++i)
        R.push_back(nullptr);
    for (unsigned int j = 0; j < columns + 1; ++j)
        C.push_back(nullptr);
}

template<typename T>
void SparseMatrix<T>::set(unsigned int rowPos, unsigned int colPos, T data) {
    auto* current = new Node<T>();
    current->data = data;
    current->posROW = rowPos;
    current->posCOL = colPos;

    if(R[rowPos] == nullptr and C[colPos] == nullptr){ //There is no Node in the ROW and COLUMN
        R[rowPos] = current;
        C[colPos] = current;
        current->next = nullptr;
        current->down = nullptr;
    } else if (R[rowPos] != nullptr and C[colPos] == nullptr){ //Row -> HAVE A NODE | Column -> NO NODE
        C[colPos] = current;

        // Updating node to the left
        unsigned int currentPosL = colPos - 1;
        auto itr = C.begin() + currentPosL;
        while(*itr == nullptr and currentPosL > 0){
            --itr;
            --currentPosL;
        }
        if(currentPosL > 0){
            (*itr)->next = current;
            // Updating node to the right
            unsigned int currentPosR = colPos + 1;
            auto itr2 = C.begin() + currentPosR;
            while(*itr2 == nullptr and currentPosR <= C.size()){
                ++itr2;
                ++currentPosR;
            }
            if(currentPosR <= C.size())
                current->next = (*itr2);
            else
                current->next = nullptr;
        }

        else{
            auto aux = R[rowPos];
            R[rowPos] = current;
            current->next = aux;
            aux = nullptr;
        }

    }else if(R[rowPos] == nullptr and C[colPos] != nullptr){ //Row -> NO NODE | Column -> HAVE A NODE
        R[rowPos] = current;

        // Updating the UP node
        unsigned int currentPosUp = rowPos - 1;
        auto itr = R.begin() + currentPosUp;
        while(*itr == nullptr and currentPosUp > 0){
            --itr;
            --currentPosUp;
        }
        if(currentPosUp > 0){
            (*itr)->down = current;
            // Updating the DOWN node
            unsigned int currentPosDown = rowPos + 1;
            auto itr2 = C.begin() + currentPosDown;
            while(*itr2 == nullptr and currentPosDown <= R.size()){
                ++itr2;
                ++currentPosDown;
            }
            if(currentPosDown <= R.size())
                current->down = (*itr2);
            else
                current->down = nullptr;
        }

        else{
            auto aux = C[colPos];
            C[colPos] = current;
            current->down = aux;
            aux = nullptr;
        }
    }else if(R[rowPos] != nullptr and C[colPos] != nullptr) { //Row -> HAVE A NODE | Column -> HAVE A NODE
//        auto* ptrCol = C[colPos];
//        auto* ptrRow = R[rowPos];
//
//        Node<T>* prev = nullptr;
//        while(ptrCol->posROW < rowPos and ptrCol->down != nullptr){
//            prev = ptrCol;
//            ptrCol = ptrCol->next;
//        }
//        if(prev != nullptr){
//            prev->down = current;
//            current->down = ptrCol;
//        } else if(ptrCol->down == nullptr) {
//            ptrCol->down = current;
//        } else {
//
//        }



    }


}

template<typename T>
T SparseMatrix<T>::get(unsigned int rowPos, unsigned int colPos) {
    auto temp = R[rowPos];
    if(temp == nullptr)
        throw std::invalid_argument("Element does not exist. ROW error.");

    while(temp->posCOL != colPos){
        temp = temp->next;
        if(temp == nullptr)
            throw std::invalid_argument("Element does not exist. COLUMN error.");
    }

    return temp->data;
}


#endif //SPARSE_MATRIX_MATRIX_H