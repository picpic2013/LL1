#include <iostream>
#include "PredictionMatrix.h"
using namespace std;

int main() {
    // define a prediction matrix
    PredictionMatrix m;
    
    // print whether there is aa -> something = { bb, ... }
    cout << m.has("aa", "bb") << endl;

    // get SOMETHING from ( aa -> something = { bb, ... } )
    cout << m.get("aa", "bb") << endl;

    // add grammar derivation ( aa -> CCC = { bb, ... } )
    m.add("aa", "bb", "CCC");

    // print whether there is aa -> something = { bb, ... }
    cout << m.has("aa", "bb") << endl;

    // get SOMETHING from ( aa -> something = { bb, ... } )
    cout << m.get("aa", "bb") << endl;
}