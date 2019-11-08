#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include <map>
#include "Node.h"
#include <iomanip>

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
    SparseMatrix(unsigned int rows, unsigned int columns);
    void set(unsigned int posROW_to_set, unsigned int posCOL_to_set, T data);
    T operator()(unsigned int rowPos, unsigned int colPos) const;
    SparseMatrix<T> operator+(SparseMatrix<T> other) const;
    SparseMatrix<T> operator-(SparseMatrix<T> other) const;
    SparseMatrix<T> operator*(T scalar) const;
    SparseMatrix<T> operator*(SparseMatrix<T> other) const;

    void print() const;

//    SparseMatrix<T> transpose() const;
//    void print() const;
//
//    ~SparseMatrix();
};


template<typename T>
SparseMatrix<T>::SparseMatrix(unsigned int rows, unsigned int columns) {
    this->rows = rows;
    this->columns = columns;

    for (unsigned int i = 0; i < rows; ++i)
        R.push_back(nullptr);
    for (unsigned int j = 0; j < columns; ++j)
        C.push_back(nullptr);
}

template<typename T>
void SparseMatrix<T>::set(unsigned int posROW_to_set, unsigned int posCOL_to_set, T data) {
    auto* current = new Node<T>();
    current->data = data;
    current->posROW = posROW_to_set;
    current->posCOL = posCOL_to_set;

    if(R[posROW_to_set - 1] == nullptr and C[posCOL_to_set - 1] == nullptr){ //There is no Node in the ROW and COLUMN
        R[posROW_to_set - 1] = current;
        C[posCOL_to_set - 1] = current;
        current->next = nullptr;
        current->down = nullptr;

    } else {

        //R[posROW_to_set - 1] = current;
        //SETTING THE COLUMNS
        if(C[posCOL_to_set - 1] == nullptr){
            C[posCOL_to_set - 1] = current;
            current->down = nullptr;
        } else {
            auto *temp = C[posCOL_to_set - 1];
            if(posROW_to_set < temp->posROW){
                auto aux = temp;
                C[posCOL_to_set - 1] = current;
                current->down = aux;
            } else {
                while(temp->down != nullptr and posROW_to_set > temp->down->posROW)
                    temp = temp->down;

                if(temp->down != nullptr){
                    auto aux = temp->down;
                    temp->down = current;
                    current->down = aux;
                } else {
                    temp->down = current;
                    current->down = nullptr;
                }
            }
        }

        //SETTING THE ROWS
        if(R[posROW_to_set - 1] == nullptr){
            R[posROW_to_set - 1] = current;
            current->next = nullptr;
        } else {
            auto *temp = R[posROW_to_set - 1];
            if(posCOL_to_set < temp->posCOL){
                auto aux = temp;
                R[posROW_to_set - 1] = current;
                current->next = aux;
            } else {
                while(temp->next != nullptr and posCOL_to_set > temp->next->posCOL)
                    temp = temp->next;

                if(temp->next != nullptr){
                    auto aux = temp->next;
                    temp->next = current;
                    current->next = aux;
                } else {
                    temp->next = current;
                    current->next = nullptr;
                }
            }
        }
    }
}

template<typename T>
T SparseMatrix<T>::operator()(unsigned int rowPos, unsigned int colPos) const {
    auto temp = R[rowPos-1];
    if(temp == nullptr)
        return 0;

    while(temp->posCOL != colPos){
        temp = temp->next;
        if(temp == nullptr)
            return 0;
    }
    return temp->data;
}

template<typename T>
void SparseMatrix<T>::print() const {
    for (int i = 1; i < rows + 1; ++i) {
        for (int j = 1; j < columns + 1; ++j) {
            cout << setw(3) << this->operator()(i,j) << " ";
        }
        cout << endl;
    }

}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator+(SparseMatrix<T> other) const {
    SparseMatrix<T> result(rows,columns);
    for (int i = 0; i < rows; ++i) {
        map<unsigned int, T> MROW;
        if(R[i] != nullptr){
            auto *temp = R[i];
            while(temp != nullptr){
                MROW[temp->posCOL-1] += temp->data;
                temp = temp->next;
            }
        }
        if(other.R[i] != nullptr){
            auto *temp = other.R[i];
            while(temp != nullptr){
                MROW[temp->posCOL-1] += temp->data;
                temp = temp->next;
            }
        }
        if(!MROW.empty())
            for(auto const & iter: MROW){
                result.set(i+1,iter.first+1,iter.second);
            }
    }
    return result;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator-(SparseMatrix<T> other) const {
    SparseMatrix<T> result(rows,columns);

    for (int i = 0; i < rows; ++i) {
        map<unsigned int, T> MROW;
        if(R[i] != nullptr){
            auto *temp = R[i];
            while(temp != nullptr){
                MROW[temp->posCOL-1] += temp->data;
                temp = temp->next;
            }
        }
        if(other.R[i] != nullptr){
            auto *temp = other.R[i];
            while(temp != nullptr){
                MROW[temp->posCOL-1] -= temp->data;
                temp = temp->next;
            }
        }
        if(!MROW.empty())
            for(auto const & iter: MROW){
                result.set(i+1,iter.first+1,iter.second);
            }
    }
    return result;

}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator*(T scalar) const {
    SparseMatrix<T> result(rows,columns);
    result = *this;

    for (int i = 0; i < rows; ++i) {
        if(result.R[i] != nullptr){
            auto *temp = result.R[i];
            while(temp != nullptr){
                temp->data = (temp->data) * scalar;
                temp = temp->next;
            }
        }
    }

    return result;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator*(SparseMatrix<T> other) const {
    SparseMatrix<T> result(rows, columns);

    for (int i = 0; i < rows; ++i) {

    }




    return result;
}


#endif //SPARSE_MATRIX_MATRIX_H