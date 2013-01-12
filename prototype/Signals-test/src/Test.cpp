#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include "signals/Slot.h"

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

struct TestSuite {
	virtual ~TestSuite(){}

	typedef int (TestSuite::*MethodType)(int);

	int notOverloadedMethod() {
	    return 0;
	}

	int notOverloadedMethodConst(int a) const {
	    return a;
	}

	double overloadedMethod(int a) {
	    return a;
	}

	double overloadedMethod(int a) const {
	    return a;
	}

	double overloadedMethod(int a, double b) {
	    return a + b;
	}

	double overloadedMethod(int a, double b) const {
	    return a + b;
	}

	float overload(int a) {
	    return a;
	}

	float overload(int a, int b) {
	    return a + b;
	}

	//TODO weak pointers to this
	//TODO nullptr exception for this
	void thisConstructorTest() {
		using namespace prototype;

		Slot<int (int)> example {this, &TestSuite::notOverloadedMethodConst};

		QU_THIS_SLOT(nonConst, notOverloadedMethod);
		QU_THIS_SLOT(constSlot, notOverloadedMethodConst);

		QU_THIS_OTHERSLOT(other, float (), notOverloadedMethod);
		QU_THIS_OTHERSLOT(otherConst, void (double), notOverloadedMethodConst);

		QU_THIS_OVERLOADSLOT(overload, overload, float (int));
		QU_THIS_OTHERSLOT(overloadOther, float (int), overload);
		QU_THIS_OTHER_OVERLOADSLOT(overloadOtherSlot, void (double), overload, float (int));

		QU_THIS_OVERLOADSLOT(overload2, overload, float (int, int));
		QU_THIS_OTHERSLOT(overloadOther2, float (int, int), overload);
		QU_THIS_OTHER_OVERLOADSLOT(overloadOtherSlot2, void (double, float), overload, float (int, int));

		QU_THIS_OTHERSLOT(complexOther, double (int), overloadedMethod);
		QU_THIS_OTHERSLOT(complexOther2, double (int, double), overloadedMethod);

		QU_THIS_OVERLOADSLOT(complexOveload, overloadedMethod, double (int));
		QU_THIS_OVERLOADSLOT(complexOveloadStrict, overloadedMethod, double (int), NonConstMethod);
		QU_THIS_OVERLOADSLOT(complexOveloadConst, overloadedMethod, double (int), ConstMethod);

		QU_THIS_OTHER_OVERLOADSLOT(other_complexOveload, void (long), overloadedMethod, double (int));
		QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadStrict, void (long), overloadedMethod, double (int), NonConstMethod);
		QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadConst, void (long), overloadedMethod, double (int), ConstMethod);

		QU_THIS_OVERLOADSLOT(complexOveload2, overloadedMethod, double (int, double));
        QU_THIS_OVERLOADSLOT(complexOveloadStrict2, overloadedMethod, double (int, double), NonConstMethod);
        QU_THIS_OVERLOADSLOT(complexOveloadConst2, overloadedMethod, double (int), ConstMethod);

        QU_THIS_OTHER_OVERLOADSLOT(other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
        QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), NonConstMethod);
        QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), ConstMethod);

//		Slot<int (int)> copySlot(thisSlot);
//		Slot<void (int)> voidCopySlot(thisSlot);
//
		ASSERT_THROWSM("Binding nullptr member",
				Slot<int (int)> nullSlot(this, (MethodType)NULL),
				prototype::BadSlotFunctionPointer*);
		ASSERT_THROWSM("Binding nullptr member",
				Slot<int (int)> nullSlot2(this, (MethodType)nullptr),
				prototype::BadSlotFunctionPointer*);
//
//		Slot<int (int)> staticSlot(&TestSuite::method1St);
//		Slot<int (int, double)> staticSlot3(&TestSuite::method2St);
//
//		Slot<int (int)> staticSlot7(TestSuite::method1St);
//		//QU_THIS_STATICSLOT(int (double), staticSlot72, method1St, int (int));
//		Slot<int (int, double)> staticSlot6(TestSuite::method2St);
//		Slot<int (int, int)> staticSlot62(TestSuite::method2St);
//
//		Slot<int (int)> staticSlot4(&foo1);
//		Slot<int (int, int)> staticSlot5(&foo2);
//
//		Functor foo;
//		Slot<int (int)> functorSlot(foo);
//		Slot<int (double)> functorSlot2(foo);
//
//		ASSERT_THROWSM("Binding nullptr member",
//				Slot<int (int)> staticSlot6((FooType)NULL),
//				prototype::BadSlotFunctionPointer*);
//		ASSERT_THROWSM("Binding nullptr member",
//				Slot<int (int)> staticSlot6((FooType)nullptr),
//				prototype::BadSlotFunctionPointer*);
	}

	void assignTest() {
	    using namespace prototype;
	    Functor foo;

//	    Slot<int (int)> thisSlot(this, &TestSuite::method1);
//	    Slot<int (int)> staticSlot(&TestSuite::method1St);
//	    Slot<int (int)> functorSlot(foo);
//
//	    thisSlot = staticSlot;
//	    staticSlot = functorSlot;
//	    functorSlot = thisSlot;

	    //QU_THIS_OVERLOADSLOT(void (int), voidSlot, method1, int (int));
	    //voidSlot = thisSlot;
	}
};



void nonThisConstructorTest() {
    TestSuite* object = new TestSuite;

    QU_SLOT(object, nonConst, notOverloadedMethod);
    QU_SLOT(object, constSlot, notOverloadedMethodConst);

    QU_OTHERSLOT(object, other, float (), notOverloadedMethod);
    QU_OTHERSLOT(object, otherConst, void (double), notOverloadedMethodConst);

    QU_OVERLOADSLOT(object, overload, overload, float (int));
    QU_OTHERSLOT(object, overloadOther, float (int), overload);
    QU_OTHER_OVERLOADSLOT(object, overloadOtherSlot, void (double), overload, float (int));

    QU_OVERLOADSLOT(object, overload2, overload, float (int, int));
    QU_OTHERSLOT(object, overloadOther2, float (int, int), overload);
    QU_OTHER_OVERLOADSLOT(object, overloadOtherSlot2, void (double, float), overload, float (int, int));

    QU_OTHERSLOT(object, complexOther, double (int), overloadedMethod);
    QU_OTHERSLOT(object, complexOther2, double (int, double), overloadedMethod);

    QU_OVERLOADSLOT(object, complexOveload, overloadedMethod, double (int));
    QU_OVERLOADSLOT(object, complexOveloadStrict, overloadedMethod, double (int), prototype::NonConstMethod);
    QU_OVERLOADSLOT(object, complexOveloadConst, overloadedMethod, double (int), prototype::ConstMethod);

    QU_OTHER_OVERLOADSLOT(object, other_complexOveload, void (long), overloadedMethod, double (int));
    QU_OTHER_OVERLOADSLOT(object, other_complexOveloadStrict, void (long), overloadedMethod, double (int), prototype::NonConstMethod);
    QU_OTHER_OVERLOADSLOT(object, other_complexOveloadConst, void (long), overloadedMethod, double (int), prototype::ConstMethod);

    QU_OVERLOADSLOT(object, complexOveload2, overloadedMethod, double (int, double));
    QU_OVERLOADSLOT(object, complexOveloadStrict2, overloadedMethod, double (int, double), prototype::NonConstMethod);
    QU_OVERLOADSLOT(object, complexOveloadConst2, overloadedMethod, double (int), prototype::ConstMethod);

    QU_OTHER_OVERLOADSLOT(object, other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
    QU_OTHER_OVERLOADSLOT(object, other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), prototype::NonConstMethod);
    QU_OTHER_OVERLOADSLOT(object, other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), prototype::ConstMethod);

    ASSERT_THROWSM("Binding nullptr member",
            prototype::Slot<int (int)> nullSlot(object, (TestSuite::MethodType)NULL),
            prototype::BadSlotFunctionPointer*);
    ASSERT_THROWSM("Binding nullptr member",
            prototype::Slot<int (int)> nullSlot2(object, (TestSuite::MethodType)nullptr),
            prototype::BadSlotFunctionPointer*);

    delete object;
}

void runSuite(){
	cute::suite s;

	s += CUTE_SMEMFUN(TestSuite, thisConstructorTest);
	s += CUTE(nonThisConstructorTest);
	s += CUTE_SMEMFUN(TestSuite, assignTest);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SignalTest");
}

int main(){
    runSuite();
    return 0;
}



