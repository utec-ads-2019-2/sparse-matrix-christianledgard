#include <iostream>
#include "SparceMatrix.h"
//#include "tester/tester.h"


int main() {
    //Tester::execute();

    SparseMatrix<int> m1(10, 10);
    m1.set(3,3,1);
    m1.set(3,5,2);
    m1.set(2,3,3);
    m1.set(3,4,4);


    //m1.set(2,7,500);



    cout << m1.get(3,3) << endl;
    cout << m1.get(3,5) << endl;
    cout << m1.get(2,3) << endl;
    cout << m1.get(3,4) << endl;
    //cout << m1.get(4,3) << endl;


    return EXIT_SUCCESS;
}