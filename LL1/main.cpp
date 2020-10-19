#include <iostream>
#include "Grammar.h"
using namespace std;

int main() {
    Grammar a;

    a.insert("S", "aBa|cc");
    a.insert("S", "cA|aaa|bbb|ccc|");
    a.insert("A", "bb");
    a.insert("B", "CCD");
    a.insert("C", "bb");
    a.insert("D", "bb");

    for (Grammar::iterator it = a.begin(); it != a.end(); it++) {
        cout << it.getKey() << "\t" << it.getValue() << endl;
    }
    cout << endl;

    cout << (--a.end()).getKey() << "\t" << (--a.end()).getValue() << endl << endl;

    Grammar b = a.autoSplitOr();

    for (Grammar::iterator it = b.begin(); it != b.end(); it++) {
        cout << it.getKey() << "\t" << it.getValue() << endl;
    }
    cout << endl;

    a.erase("S", "cA");

    for (Grammar::iterator it = a.begin(); it != a.end(); it++) {
        cout << it.getKey() << "\t" << it.getValue() << endl;
    }
    cout << endl;

    return 0;
}