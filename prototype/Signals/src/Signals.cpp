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
//using namespace std;

using namespace prototype;
using std::cout;
using std::endl;

void getConvert(Slot<int (float)> value) {

}

int main() {

	cout << std::is_assignable<Slot<int (float)>, Slot<void (int)> >::value << endl;
	cout << std::is_assignable<Slot<void (int)>, Slot<int (float)> >::value << endl;
	cout << std::is_assignable<Slot<void (int)>, std::string>::value << endl;
	cout << std::is_convertible<Slot<void (int)>, std::string>::value << endl << endl;

	cout << std::is_convertible<std::function<int (float)>, std::function<void (int)> >::value << endl;
	cout << std::is_convertible<std::function<void (int)>, std::function<int (float)> >::value << endl;

	cout << std::is_constructible<std::function<int (float)>, std::function<void (int)> >::value << endl;
	cout << std::is_constructible<std::function<void (int)>, std::function<int (float)> >::value << endl;

	Slot<int (float)> slot1;
	Slot<void (int)> slot2;
	slot2 = slot1;
	//slot1 = slot2;
	//Slot<int (float)> slot3(slot2);
	//getConvert(slot2);
	return 0;
}

//Slot<int (float)> convert(Slot<void (int)> value) {
//    return value;
//}
