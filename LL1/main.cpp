#include <iostream>
#include "Grammar.h"
using namespace std;

int main() {
    Grammar a;

    a.insert("S", "aBa|cc", false);
    a.insert("S", "cA|aaa|bbb|ccc|", false);
    a.insert("A", "bb", false);
    a.insert("B", "CCD", false);
    a.insert("C", "bb", false);
    a.insert("D", "bb", false);

    set<char> temp = a.getFirst("S");
    for (char i : temp) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}