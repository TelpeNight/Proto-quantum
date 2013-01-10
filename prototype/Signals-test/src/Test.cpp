#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include "signals/Slot.h"

static int foo1(int a) {
	return a;
}

static double foo2(int a, double b) {
	return a + b;
}

typedef int (*FooType)(int);


class Functor {
public:
	int operator()(int a) {
		return a;
	}

	int operator()(double a) {
		return 0xff;
	}
};

prototype::Slot<int (int)> getTempSlot() {
	return prototype::Slot<int (int)>(foo1);
}

struct TestSuite {
	virtual ~TestSuite(){}

	typedef int (TestSuite::*MethodType)(int);

	static int method1St(int a) {
		return a;
	}

	static int method1St(int a, double b) {
	        return a;
	    }

	static double method2St(int a, double b) {
		return a + b;
	}

	virtual int method1(int a) {
		return a;
	}

	int method1(double a) {
	        return a;
	    }

	double method2(int a, double b) const {
		return a + b;
	}

	void constructorTest() {
		using namespace prototype;

		Slot<int (int)> thisSlot(this, &TestSuite::method1);
		Slot<int (int, int)> thisSlot2(this, &TestSuite::method2);
		Slot<int (int, double)> thisSlot3(this, &TestSuite::method2);
		QU_THIS_OVERLOADSLOT(void (int), voidSlot, method1, int (int));

		Slot<int (int)> copySlot(thisSlot);
		Slot<void (int)> voidCopySlot(thisSlot);

		ASSERT_THROWSM("Binding nullptr member",
				Slot<int (int)> nullSlot(this, (MethodType)NULL),
				prototype::BadSlotFunctionPointer*);
		ASSERT_THROWSM("Binding nullptr member",
				Slot<int (int)> nullSlot2(this, (MethodType)nullptr),
				prototype::BadSlotFunctionPointer*);

		Slot<int (int)> staticSlot(&TestSuite::method1St);
		Slot<int (int, double)> staticSlot3(&TestSuite::method2St);

		Slot<int (int)> staticSlot7(TestSuite::method1St);
		QU_THIS_STATICSLOT(int (double), staticSlot72, method1St, int (int));
		Slot<int (int, double)> staticSlot6(TestSuite::method2St);
		Slot<int (int, int)> staticSlot62(TestSuite::method2St);

		Slot<int (int)> staticSlot4(&foo1);
		Slot<int (int, int)> staticSlot5(&foo2);

		Functor foo;
		Slot<int (int)> functorSlot(foo);
		Slot<int (double)> functorSlot2(foo);

		ASSERT_THROWSM("Binding nullptr member",
				Slot<int (int)> staticSlot6((FooType)NULL),
				prototype::BadSlotFunctionPointer*);
		ASSERT_THROWSM("Binding nullptr member",
				Slot<int (int)> staticSlot6((FooType)nullptr),
				prototype::BadSlotFunctionPointer*);
	}

	void assignTest() {
	    using namespace prototype;
	    Functor foo;

	    Slot<int (int)> thisSlot(this, &TestSuite::method1);
	    Slot<int (int)> staticSlot(&TestSuite::method1St);
	    Slot<int (int)> functorSlot(foo);

	    thisSlot = staticSlot;
	    staticSlot = functorSlot;
	    functorSlot = thisSlot;

	    QU_THIS_OVERLOADSLOT(void (int), voidSlot, method1, int (int));
	    voidSlot = thisSlot;
	}
};

void runSuite(){
	cute::suite s;

	s += CUTE_SMEMFUN(TestSuite, constructorTest);
	s += CUTE_SMEMFUN(TestSuite, assignTest);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SignalTest");
}

int main(){
    runSuite();
    return 0;
}



