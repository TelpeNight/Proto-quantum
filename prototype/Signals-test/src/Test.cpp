#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include "signals/Slot.h"
#include "signals/SlotShortcuts.h"

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

static int notOverloadedFunction() {
        return 0;
    }

static float overloadFunction(int a) {
    return a;
}

static float overloadFunction(int a, int b) {
    return a + b;
}

struct TestSuite {
	virtual ~TestSuite(){}

	typedef int (TestSuite::*MethodType)(int);

	static int notOverloadedMethodSt() {
        return 0;
    }

    static float overloadSt(int a) {
        return a;
    }

    static float overloadSt(int a, int b) {
        return a + b;
    }

    //----------------------------------------//

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

	void defaultConstructorTest() {
	    using namespace prototype;
	    Slot<int ()> slot;
	    Slot<int ()> slot2(nullptr);
	}

	//TODO weak pointers to this
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
				Slot<int (int)> nullSlot(this, (MethodType)nullptr),
				prototype::BadSlotFunctionPointer*);

		ASSERT_THROWSM("Binding nullptr instance",
                Slot<int ()> nullSlot((TestSuite*)NULL, &TestSuite::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                Slot<int ()> nullSlot((TestSuite*)nullptr, &TestSuite::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
//
//		Functor foo;
//		Slot<int (int)> functorSlot(foo);
//		Slot<int (double)> functorSlot2(foo);
	}

	//TODO {} in constructors
	void staticScopeSlotTest() {
	    QU_STATIC_SLOT(staticSLot, notOverloadedMethodSt);
	    QU_STATIC_SLOT(staticSLotPtr, &notOverloadedMethodSt);

	    QU_STATIC_OTHERSLOT(other_staticSLot, void (), notOverloadedMethodSt);
	    QU_STATIC_OTHERSLOT(other_staticSLotPtr, double (), &notOverloadedMethodSt);

	    QU_STATIC_OVERLOADSLOT(overload1, overloadSt, float (int));
	    QU_STATIC_OTHERSLOT(other_overload1, float (int), overloadSt);
	    QU_STATIC_OVERLOADSLOT(overload1Ptr, &overloadSt, float (int));
	    QU_STATIC_OTHERSLOT(other_overload1Ptr, float (int), &overloadSt);

	    QU_STATIC_OVERLOADSLOT(overload2, overloadSt, float (int, int));
        QU_STATIC_OTHERSLOT(other_overload2, float (int, int), overloadSt);
        QU_STATIC_OVERLOADSLOT(overload2Ptr, &overloadSt, float (int, int));
        QU_STATIC_OTHERSLOT(other_overload2Ptr, float (int, int), &overloadSt);

        QU_STATIC_OTHER_OVERLOADSLOT(overload1_other, void (float), overloadSt, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload1Ptr_other, float (float), &overloadSt, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload2_other, void (float ,double), overloadSt, float (int, int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload2Ptr_other, int (int, int), &overloadSt, float (int, int));

        prototype::Slot<int (int)> staticSlotNULL((FooType)NULL);
        prototype::Slot<int (int)> staticSlotNullPtr((FooType)nullptr);
	}

	void staticSlotTest() {
        QU_STATIC_SLOT(staticSLot, notOverloadedFunction);
        QU_STATIC_SLOT(staticSLotPtr, &notOverloadedFunction);

        QU_STATIC_OTHERSLOT(other_staticSLot, void (), notOverloadedFunction);
        QU_STATIC_OTHERSLOT(other_staticSLotPtr, double (), &notOverloadedFunction);

        QU_STATIC_OVERLOADSLOT(overload1, overloadFunction, float (int));
        QU_STATIC_OTHERSLOT(other_overload1, float (int), overloadFunction);
        QU_STATIC_OVERLOADSLOT(overload1Ptr, &overloadFunction, float (int));
        QU_STATIC_OTHERSLOT(other_overload1Ptr, float (int), &overloadFunction);

        QU_STATIC_OVERLOADSLOT(overload2, overloadFunction, float (int, int));
        QU_STATIC_OTHERSLOT(other_overload2, float (int, int), overloadFunction);
        QU_STATIC_OVERLOADSLOT(overload2Ptr, &overloadFunction, float (int, int));
        QU_STATIC_OTHERSLOT(other_overload2Ptr, float (int, int), &overloadFunction);

        QU_STATIC_OTHER_OVERLOADSLOT(overload1_other, void (float), overloadFunction, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload1Ptr_other, float (float), &overloadFunction, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload2_other, void (float ,double), overloadFunction, float (int, int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload2Ptr_other, int (int, int), &overloadFunction, float (int, int));

        prototype::Slot<int (int)> staticSlotNULL((FooType)NULL);
        prototype::Slot<int (int)> staticSlotNullPtr((FooType)nullptr);
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

    ASSERT_THROWSM("Binding NULL member",
            prototype::Slot<int (int)> nullSlot(object, (TestSuite::MethodType)NULL),
            prototype::BadSlotFunctionPointer*);
    ASSERT_THROWSM("Binding nullptr member",
            prototype::Slot<int (int)> nullSlot2(object, (TestSuite::MethodType)nullptr),
            prototype::BadSlotFunctionPointer*);

    ASSERT_THROWSM("Binding nullptr instance",
            prototype::Slot<int ()> nullSlot((TestSuite*)NULL, &TestSuite::notOverloadedMethod),
            prototype::BadSlotInstancePointer*);
    ASSERT_THROWSM("Binding nullptr instance",
            prototype::Slot<int ()> nullSlot((TestSuite*)nullptr, &TestSuite::notOverloadedMethod),
            prototype::BadSlotInstancePointer*);

    delete object;
}

void staticSlotTest() {
    QU_STATIC_SLOT(staticSLot, TestSuite::notOverloadedMethodSt);
    QU_STATIC_SLOT(staticSLotPtr, &TestSuite::notOverloadedMethodSt);

    QU_STATIC_OTHERSLOT(other_staticSLot, void (), TestSuite::notOverloadedMethodSt);
    QU_STATIC_OTHERSLOT(other_staticSLotPtr, double (), &TestSuite::notOverloadedMethodSt);

    QU_STATIC_OVERLOADSLOT(overload1, TestSuite::overloadSt, float (int));
    QU_STATIC_OTHERSLOT(other_overload1, float (int), TestSuite::overloadSt);
    QU_STATIC_OVERLOADSLOT(overload1Ptr, &TestSuite::overloadSt, float (int));
    QU_STATIC_OTHERSLOT(other_overload1Ptr, float (int), &TestSuite::overloadSt);

    QU_STATIC_OVERLOADSLOT(overload2, TestSuite::overloadSt, float (int, int));
    QU_STATIC_OTHERSLOT(other_overload2, float (int, int), TestSuite::overloadSt);
    QU_STATIC_OVERLOADSLOT(overload2Ptr, &TestSuite::overloadSt, float (int, int));
    QU_STATIC_OTHERSLOT(other_overload2Ptr, float (int, int), &TestSuite::overloadSt);

    QU_STATIC_OTHER_OVERLOADSLOT(overload1_other, void (float), TestSuite::overloadSt, float (int));
    QU_STATIC_OTHER_OVERLOADSLOT(overload1Ptr_other, float (float), &TestSuite::overloadSt, float (int));
    QU_STATIC_OTHER_OVERLOADSLOT(overload2_other, void (float ,double), TestSuite::overloadSt, float (int, int));
    QU_STATIC_OTHER_OVERLOADSLOT(overload2Ptr_other, int (int, int), &TestSuite::overloadSt, float (int, int));


    prototype::Slot<int (int)> staticSlotNULL((FooType)NULL);
    prototype::Slot<int (int)> staticSlotNullPtr((FooType)nullptr);
}

void runSuite(){
	cute::suite s;

	s += CUTE_SMEMFUN(TestSuite, defaultConstructorTest);
	s += CUTE_SMEMFUN(TestSuite, thisConstructorTest);
	s += CUTE(nonThisConstructorTest);
	s += CUTE_SMEMFUN(TestSuite, staticScopeSlotTest);
	s += CUTE(staticSlotTest);
	s += CUTE_SMEMFUN(TestSuite, staticSlotTest);
	s += CUTE_SMEMFUN(TestSuite, assignTest);

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SignalTest");
}

int main(){
    runSuite();
    return 0;
}



