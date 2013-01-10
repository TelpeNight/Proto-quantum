//============================================================================
// Name        : Signals.cpp
// Author      : Night
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "signals/Slot.h"
using namespace std;

using prototype::Slot;
using std::cout;
using std::endl;

class FakeComponent {
public :
	int method(int a) {
		return a;
	}

	typedef int (FakeComponent::*MethodType)(int);
	MethodType getMethod() {
		return &FakeComponent::method;
	}
};

template<class T>
struct FakeMethodScope : public T {
};

int foo(int s) {
	return s;
}

typedef int (*Foo)(int);


int main() {

	auto pComponent = new FakeComponent();
	Slot<int (int)> slot(pComponent, &FakeComponent::method);
	cout << !std::is_same<
			typename std::remove_reference<Foo>::type,
			typename std::remove_reference<Slot<int (int)> >::type>::value << std::endl;
	cout << slot(9) << endl;
	return 0;
}
