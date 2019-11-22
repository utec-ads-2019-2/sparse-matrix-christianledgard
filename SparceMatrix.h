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
    unsigned int rows, columns;
    vector <Node<T>*> R;
    vector <Node<T>*> C;
    bool nodeExistInPos(unsigned int posROW, unsigned int posCOL);
    Node<T>* findNode(unsigned int posROW, unsigned int posCOL);


public:
    SparseMatrix(unsigned int rows, unsigned int columns);
    SparseMatrix(const SparseMatrix &object);
    void set(unsigned int posROW_to_set, unsigned int posCOL_to_set, T data);
    T operator()(unsigned int rowPos, unsigned int colPos) const;
    SparseMatrix<T> operator+(SparseMatrix<T> other) const;
    SparseMatrix<T> operator-(SparseMatrix<T> other) const;
    SparseMatrix<T> operator*(T scalar) const;
    SparseMatrix<T> operator*(SparseMatrix<T> other) const;
    SparseMatrix<T> transpose() const;
    SparseMatrix<T>& operator=(const SparseMatrix<T> &other);
    void clear();
    virtual ~SparseMatrix(){clear();}
    void print() const;



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
    // Dónde borras en caso el dato sea 0?
    auto* current = new Node<T>();
    current->data = data;
    current->posROW = posROW_to_set;
    current->posCOL = posCOL_to_set;

    if(R[posROW_to_set] == nullptr and C[posCOL_to_set] == nullptr){ //There is no Node in the ROW and COLUMN
        R[posROW_to_set] = current;
        C[posCOL_to_set] = current;
        current->next = nullptr;
        current->down = nullptr;

    } else {
        //R[posROW_to_set - 1] = current;
        //SETTING THE COLUMNS
        if(C[posCOL_to_set] == nullptr){
            C[posCOL_to_set] = current;
            current->down = nullptr;
        } else {
            auto *temp = C[posCOL_to_set];
            if(posROW_to_set < temp->posROW){
                auto aux = temp;
                C[posCOL_to_set] = current;
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
        if(R[posROW_to_set] == nullptr){
            R[posROW_to_set] = current;
            current->next = nullptr;
        } else {
            auto *temp = R[posROW_to_set];
            if(posCOL_to_set < temp->posCOL){
                auto aux = temp;
                R[posROW_to_set] = current;
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
    auto temp = R[rowPos];
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
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
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
                MROW[temp->posCOL] += temp->data;
                temp = temp->next;
            }
        }
        if(other.R[i] != nullptr){
            auto *temp = other.R[i];
            while(temp != nullptr){
                MROW[temp->posCOL] += temp->data;
                temp = temp->next;
            }
        }
        if(!MROW.empty())
            for(auto const & iter: MROW){
                result.set(i,iter.first,iter.second);
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
                MROW[temp->posCOL] += temp->data;
                temp = temp->next;
            }
        }
        if(other.R[i] != nullptr){
            auto *temp = other.R[i];
            while(temp != nullptr){
                MROW[temp->posCOL] -= temp->data;
                temp = temp->next;
            }
        }
        if(!MROW.empty())
            for(auto const & iter: MROW){
                result.set(i,iter.first,iter.second);
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

// Utilizar los métodos de la matriz va a causar que tu multiplicación sea más lenta
template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator*(SparseMatrix<T> other) const {
    SparseMatrix<T> result(rows, columns);

//    if(other.Rows != columns )
//        throw out_of_range("Check your input");

    for(int i=0; i < rows; i++) {
        for (int j = 0; j < other.columns; j++) {
            T cont = 0;
            for (int k = 0; k < columns; k++)
                cont += operator()(i, k) * other.operator()(k, j);
            result.set(i, j, cont);
        }
    }
    return result;
}

template<typename T>
SparseMatrix<T> &SparseMatrix<T>::operator=(const SparseMatrix<T> &other) {

        clear();
        rows = other.rows;
        columns = other.columns;

        for (unsigned int i = 0; i < rows; ++i)
            R.push_back(nullptr);
        for (unsigned int j = 0; j < columns; ++j)
            C.push_back(nullptr);

        for (int i = 0; i < rows ; ++i) {
            for (int j = 0; j < columns; ++j) {
                set(i, j, other.operator()(i, j));
            }
        }

    return *this;
}

template<typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix &object) {
    rows = object.rows;
    columns = object.columns;
    for (unsigned int i = 0; i < rows; ++i)
        R.push_back(nullptr);
    for (unsigned int j = 0; j < columns; ++j)
        C.push_back(nullptr);

    for (int i = 0; i < rows ; ++i) {
        for (int j = 0; j < columns; ++j) {
            set(i, j, object.operator()(i, j));
        }
    }
}

template<typename T>
void SparseMatrix<T>::clear() {
    while (!R.empty()) {
        auto currentNode = R.back();
        while (currentNode != nullptr) {
            auto temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
        R.pop_back();
    }
    while (!C.empty())
        C.pop_back();
//    for (int i = 0; i < rows; ++i){
//        auto temp = R[i];
//        if(temp != nullptr){
//            while(temp != nullptr){
//                auto te = temp;
//                temp = temp->next;
//                delete te;
//            }
//            R[i] = nullptr;
//        }
//    }
//    for (int j = 0; j < columns; ++j) {
//        C[j] = nullptr;
//    }
//    R.clear(); C.clear();
//    rows = columns = 0;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::transpose() const {
    SparseMatrix<T> result(rows, columns);
    result = *this;

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            result.set(j,i,this->operator()(i,j));

    return result;
}

template<typename T>
bool SparseMatrix<T>::nodeExistInPos(unsigned int posROW, unsigned int posCOL) {
    auto temp = R[posROW];
    if(temp != nullptr){
        while(temp != nullptr){
            if(temp->posCOL == posCOL) return true;
            temp = temp->next;
        }
    }
    return false;
}

template<typename T>
Node<T> *SparseMatrix<T>::findNode(unsigned int posROW, unsigned int posCOL) {
    auto temp = R[posROW];
    if(temp != nullptr){
        while(temp != nullptr){
            if(temp->posCOL == posCOL) return temp;
            temp = temp->next;
        }
    }
    return nullptr;
}


#endif //SPARSE_MATRIX_MATRIX_H