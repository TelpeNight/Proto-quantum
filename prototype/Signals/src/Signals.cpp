//============================================================================
// Name        : Signals.cpp
// Author      : Night
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "signals/Slot.h"
#include "signals/TemplateHelpers.h"
using namespace std;

using namespace prototype;
using std::cout;
using std::endl;

int main() {

	cout << One<
	    std::is_same<
            RemoveRef<Slot<int (int)>>,
            Slot<void (int)>
	    >
	>::value << endl;
	cout <<  std::is_same<
            RemoveRef<Slot<int (int)>>,
            Slot<void (int)>
        >::value << endl;
	return 0;
}
