#include <iostream>
#include "SparceMatrix.h"
#include "tester/tester.h"

template <typename T>
ostream& operator<<(ostream& os,SparseMatrix<T> &matrix);

int main() {
    Tester::execute();

    SparseMatrix<int> m1(5, 5);
    SparseMatrix<int> m2(5, 5);
    SparseMatrix<int> m3(5, 5);

    m1.set(0,1,1);
    m1.set(1,2,2);
    m1.set(2,1,3);
    m1.set(3,2,4);
    m1.set(3,4,4);
    m1.set(2,3,500);

    m2.set(0,1,1);
    m2.set(1,2,5);
    m2.set(2,1,4);
    m2.set(3,4,8);
    m2.set(2,3,600);

    
    cout << m1; //<< m2;
    //cout << m2;


    m3 = m1.transpose();

    cout << m3;



    // cout << m1 ;//<< m2;


    //cout << m1.get(4,3) << endl;


    return EXIT_SUCCESS;
}

template <typename T>
ostream& operator<<(ostream& os,SparseMatrix<T> &matrix){
    matrix.print();
    os << endl;
    return os;
}
