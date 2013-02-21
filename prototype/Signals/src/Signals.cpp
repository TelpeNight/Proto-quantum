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
#include "signals/type_traits.h"
//using namespace std;

using namespace prototype;
using std::cout;
using std::endl;

int main() {

	cout << std::is_assignable<Slot<int (float)>, Slot<void (int)>>::value << endl;
	cout << std::is_assignable<Slot<void (int)>, Slot<int (int)>>::value << endl;
	cout << std::is_assignable<Slot<void (int)>, std::string>::value << endl;
	cout << std::is_convertible<Slot<void (int)>, std::string>::value << endl << endl;

	cout << prototype::is_convertible<Slot<int (float)>, Slot<void (int)>>::value << endl;
	cout << prototype::is_convertible<Slot<void (int)>, Slot<int (int)>>::value << endl;

	Slot<int (float)> slot1;
	Slot<void (int)> slot2;
	slot2 = slot1;
	//slot1 = slot2;
	return 0;
}
