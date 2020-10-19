#include <iostream>
#include "Grammar.h"
using namespace std;

int main() {
    Grammar a;

    a.insert("S", "aBa");
    a.insert("S", "cA");
    a.insert("A", "bb");
    a.insert("B", "CCD");
    a.insert("C", "bb");
    a.insert("D", "bb");

    for (Grammar::iterator it = a.begin(); it != a.end(); it++) {
        cout << it.getKey() << "\t" << it.getValue() << endl;
    }
    cout << endl;

    cout << (a.begin() + 2).getKey() << "\t" << (a.begin() + 2).getValue() << endl;
    cout << (a.end() - 2).getKey() << "\t" << (a.end() - 2).getValue() << endl;

    return 0;
}