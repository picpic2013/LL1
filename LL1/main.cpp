#include <iostream>
#include "Grammar1.h"
#include "Grammar2.h"
#include "PredictionMatrix.h"

using namespace std;

int main() {

    Grammar1 a('S');
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

    //a.insert("E", "E+E|E*E|a");
    //用'|'分割
    a.autoSplitOrInPlace();

    cout << a << endl;

    //利用集装箱将a转换成Grammar2类的a2
    Grammar2 a2 = a.getGrammar2();
    //消除左递归
    if (a2.if_left_recursion()) {
        a2.remove_left_recursion();
    }

    //消除左因子
    if (a2.if_left_factor())
        a2.remove_left_factor();

    //转换成Grammar1，并且判断是否是LL1文法，
    //若是LL1文法，则已经完成了求解Select集合的过程
    Grammar1 a1(a2);
    if (a1.isLL1()) {
        cout << "该文法是LL1文法" << endl;
    }
    else {
        cout << "该文法不是LL1文法，无法处理" << endl;
        return 0;
    }

    //当文法是LL1(1)文法的时候，对于输入的字符串input，进行判别
    string input;
    while (cin >> input) {
        if (a1.fitGrammar(input)) {
            cout << "该句子属于该文法" << endl;
        }
        else {
            cout << "该句子不属于该文法" << endl;
        }
    }

    return 0;
}