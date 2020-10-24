#include <iostream>
#include "Grammar.h"
using namespace std;

int main() {

    Grammar a('S');
    /*
    a.insert("S", "aBa|cc", false);
    a.insert("S", "cA|aaa|bbb|ccc|@", false);
    a.insert("A", "bb", false);
    a.insert("B", "CCD", false);
    a.insert("C", "bb", false);
    a.insert("D", "bb", false);
    */

    /* 
E->TA
A->+TA|@
B->*FB|@
F->i|(E)
T->FB
    */

    /*
    a.insert("E", "TA");
    a.insert("A", "+TA|@");
    a.insert("T", "FB");
    a.insert("B", "*FB|@");
    a.insert("F", "i|(E)");
    a.autoSplitOrInPlace();
    */
    
    // cin >> a;

    a.insert("S", "MH|a");
    a.insert("H", "LSo|@");
    a.insert("K", "dML|@");
    a.insert("L", "eHf");
    a.insert("M", "K|bLM");

    a.autoSplitOrInPlace();

    cout << a << endl;

    cout << a.isLL1() << endl;

    for (auto it = a.begin(); it != a.end(); ++it) {
        for (char i : a.getSelect(it.getKey(), it.getValue())) {
            cout << i << " ";
        }
        cout << endl;
    }
    
    return 0;
}