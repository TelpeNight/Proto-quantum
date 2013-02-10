#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include "signals/Slot.h"
#include "signals/SlotShortcuts.h"
#include <memory>
#include "utils/make_unique.h"

typedef int (*FooType)(int);


class Functor {
public:
	int operator()(int a) {
		return a;
	}
};

class OverLoadedFunctor {
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

struct TestClass {

    typedef int (TestClass::*MethodType)(int);
    typedef int (TestClass::*ConstMethodType)(int) const;

    virtual ~TestClass(){}

    typedef std::shared_ptr<TestClass> shared;
    typedef std::weak_ptr<TestClass> weak;
    typedef std::unique_ptr<TestClass> unique;

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

    //------------ Some scoped test cases------------//
    void thisConstructorTest() {
        using namespace prototype;

        Slot<int (int)> example {this, &TestClass::notOverloadedMethodConst};

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

        ASSERT_THROWSM("Binding nullptr member",
                Slot<int (int)> nullSlot(this, (MethodType)NULL),
                prototype::BadSlotFunctionPointer*);
        ASSERT_THROWSM("Binding nullptr member",
                Slot<int (int)> nullSlot(this, (MethodType)nullptr),
                prototype::BadSlotFunctionPointer*);

        ASSERT_THROWSM("Binding nullptr instance",
                Slot<int ()> nullSlot((TestClass*)NULL, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                Slot<int ()> nullSlot((TestClass*)nullptr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
    }

    void thisConstructorTestConst() const {
        using namespace prototype;

        Slot<int (int)> example {this, &TestClass::notOverloadedMethodConst};

        QU_THIS_SLOT(constSlot, notOverloadedMethodConst);
        QU_THIS_OTHERSLOT(otherConst, void (double), notOverloadedMethodConst);

        //QU_THIS_OVERLOADSLOT(complexOveload, overloadedMethod, double (int));
        //QU_THIS_OVERLOADSLOT(complexOveloadStrict, overloadedMethod, double (int), NonConstMethod);
        QU_THIS_OVERLOADSLOT(complexOveloadConst, overloadedMethod, double (int), ConstMethod);

        //QU_THIS_OTHER_OVERLOADSLOT(other_complexOveload, void (long), overloadedMethod, double (int));
        //QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadStrict, void (long), overloadedMethod, double (int), NonConstMethod);
        QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadConst, void (long), overloadedMethod, double (int), ConstMethod);

        //QU_THIS_OVERLOADSLOT(complexOveload2, overloadedMethod, double (int, double));
        //QU_THIS_OVERLOADSLOT(complexOveloadStrict2, overloadedMethod, double (int, double), NonConstMethod);
        QU_THIS_OVERLOADSLOT(complexOveloadConst2, overloadedMethod, double (int), ConstMethod);

        //QU_THIS_OTHER_OVERLOADSLOT(other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
        //QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), NonConstMethod);
        QU_THIS_OTHER_OVERLOADSLOT(other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), ConstMethod);

        ASSERT_THROWSM("Binding nullptr member",
                Slot<int (int)> nullSlot(this, (ConstMethodType)NULL),
                prototype::BadSlotFunctionPointer*);
        ASSERT_THROWSM("Binding nullptr member",
                Slot<int (int)> nullSlot(this, (ConstMethodType)nullptr),
                prototype::BadSlotFunctionPointer*);

        ASSERT_THROWSM("Binding nullptr instance",
                Slot<int ()> nullSlot((TestClass*)NULL, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                Slot<int ()> nullSlot((TestClass*)nullptr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
    }


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
};

struct TestSuite {

    //----------Context----------//
    TestClass::unique instance = std::make_unique<TestClass>();
    TestClass::shared tempPtr = std::make_shared<TestClass>();
    TestClass::weak ptr = tempPtr;
    TestClass::weak nullPtr;

    prototype::Slot<int ()> emptySlot;
    prototype::Slot<int (int)> nonEmptyInstanceSlot = {instance.get(),  &TestClass::notOverloadedMethodConst};
    prototype::Slot<int (int)> deletedSlot = {ptr, &TestClass::notOverloadedMethodConst};
    //---------------------------//

	/////////////////////////////////////////////////////////

	void defaultConstructorTest() {
	    using namespace prototype;
	    Slot<int ()> slot;
	    Slot<int ()> slot2(nullptr);
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

	void functorConstructorTest() {
	    QU_STATIC_SLOT(functorSlot, Functor());
	    QU_STATIC_OTHERSLOT(other_staticSLot, void (float), Functor());

	    QU_STATIC_OVERLOADSLOT(overload1, OverLoadedFunctor(), int (int));
	    QU_STATIC_OTHERSLOT(other_overload1, int (int), OverLoadedFunctor());

	    QU_STATIC_OVERLOADSLOT(overload2, OverLoadedFunctor(), float (double));
	    QU_STATIC_OTHERSLOT(other_overload2, float (double), OverLoadedFunctor());

	    QU_STATIC_OTHER_OVERLOADSLOT(overload1_other, void (float), OverLoadedFunctor(), int (int));
	    QU_STATIC_OTHER_OVERLOADSLOT(overload2_other, void (float), OverLoadedFunctor(), int (double));
	}

	void weakPointerConstructor() {
	    QU_SLOT(ptr, nonConst, notOverloadedMethod);
        QU_SLOT(ptr, constSlot, notOverloadedMethodConst);

        QU_OTHERSLOT(ptr, other, float (), notOverloadedMethod);
        QU_OTHERSLOT(ptr, otherConst, void (double), notOverloadedMethodConst);

        QU_OVERLOADSLOT(ptr, overload, overload, float (int));
        QU_OTHERSLOT(ptr, overloadOther, float (int), overload);
        QU_OTHER_OVERLOADSLOT(ptr, overloadOtherSlot, void (double), overload, float (int));

        QU_OVERLOADSLOT(ptr, overload2, overload, float (int, int));
        QU_OTHERSLOT(ptr, overloadOther2, float (int, int), overload);
        QU_OTHER_OVERLOADSLOT(ptr, overloadOtherSlot2, void (double, float), overload, float (int, int));

        QU_OTHERSLOT(ptr, complexOther, double (int), overloadedMethod);
        QU_OTHERSLOT(ptr, complexOther2, double (int, double), overloadedMethod);

        QU_OVERLOADSLOT(ptr, complexOveload, overloadedMethod, double (int));
        QU_OVERLOADSLOT(ptr, complexOveloadStrict, overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OVERLOADSLOT(ptr, complexOveloadConst, overloadedMethod, double (int), prototype::ConstMethod);

        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveload, void (long), overloadedMethod, double (int));
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadStrict, void (long), overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadConst, void (long), overloadedMethod, double (int), prototype::ConstMethod);

        QU_OVERLOADSLOT(ptr, complexOveload2, overloadedMethod, double (int, double));
        QU_OVERLOADSLOT(ptr, complexOveloadStrict2, overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OVERLOADSLOT(ptr, complexOveloadConst2, overloadedMethod, double (int), prototype::ConstMethod);

        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), prototype::ConstMethod);

        ASSERT_THROWSM("Binding NULL member",
                prototype::Slot<int (int)> nullSlot(ptr, (TestClass::MethodType)NULL),
                prototype::BadSlotFunctionPointer*);
        ASSERT_THROWSM("Binding nullptr member",
                prototype::Slot<int (int)> nullSlot2(ptr, (TestClass::MethodType)nullptr),
                prototype::BadSlotFunctionPointer*);

        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot(nullPtr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot(nullPtr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
	}

	void sharedPointerConstructor() {

        QU_SLOT(ptr, nonConst, notOverloadedMethod);
        QU_SLOT(ptr, constSlot, notOverloadedMethodConst);

        QU_OTHERSLOT(ptr, other, float (), notOverloadedMethod);
        QU_OTHERSLOT(ptr, otherConst, void (double), notOverloadedMethodConst);

        QU_OVERLOADSLOT(ptr, overload, overload, float (int));
        QU_OTHERSLOT(ptr, overloadOther, float (int), overload);
        QU_OTHER_OVERLOADSLOT(ptr, overloadOtherSlot, void (double), overload, float (int));

        QU_OVERLOADSLOT(ptr, overload2, overload, float (int, int));
        QU_OTHERSLOT(ptr, overloadOther2, float (int, int), overload);
        QU_OTHER_OVERLOADSLOT(ptr, overloadOtherSlot2, void (double, float), overload, float (int, int));

        QU_OTHERSLOT(ptr, complexOther, double (int), overloadedMethod);
        QU_OTHERSLOT(ptr, complexOther2, double (int, double), overloadedMethod);

        QU_OVERLOADSLOT(ptr, complexOveload, overloadedMethod, double (int));
        QU_OVERLOADSLOT(ptr, complexOveloadStrict, overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OVERLOADSLOT(ptr, complexOveloadConst, overloadedMethod, double (int), prototype::ConstMethod);

        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveload, void (long), overloadedMethod, double (int));
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadStrict, void (long), overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadConst, void (long), overloadedMethod, double (int), prototype::ConstMethod);

        QU_OVERLOADSLOT(ptr, complexOveload2, overloadedMethod, double (int, double));
        QU_OVERLOADSLOT(ptr, complexOveloadStrict2, overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OVERLOADSLOT(ptr, complexOveloadConst2, overloadedMethod, double (int), prototype::ConstMethod);

        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(ptr, other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), prototype::ConstMethod);

        ASSERT_THROWSM("Binding NULL member",
                prototype::Slot<int (int)> nullSlot(ptr, (TestClass::MethodType)NULL),
                prototype::BadSlotFunctionPointer*);
        ASSERT_THROWSM("Binding nullptr member",
                prototype::Slot<int (int)> nullSlot2(ptr, (TestClass::MethodType)nullptr),
                prototype::BadSlotFunctionPointer*);

        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot(nullPtr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot(nullPtr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
    }

	void nonThisConstructorTest() {
        auto object = instance.get();

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
                prototype::Slot<int (int)> nullSlot(object, (TestClass::MethodType)NULL),
                prototype::BadSlotFunctionPointer*);
        ASSERT_THROWSM("Binding nullptr member",
                prototype::Slot<int (int)> nullSlot2(object, (TestClass::MethodType)nullptr),
                prototype::BadSlotFunctionPointer*);

        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot((TestClass*)NULL, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot((TestClass*)nullptr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
    }

    void nonThisConstructorTestConst() {
        const TestClass* object = instance.get();

        QU_SLOT(object, constSlot, notOverloadedMethodConst);
        QU_OTHERSLOT(object, otherConst, void (double), notOverloadedMethodConst);

        //QU_OVERLOADSLOT(object, complexOveload, overloadedMethod, double (int));
        //QU_OVERLOADSLOT(object, complexOveloadStrict, overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OVERLOADSLOT(object, complexOveloadConst, overloadedMethod, double (int), prototype::ConstMethod);

        //QU_OTHER_OVERLOADSLOT(object, other_complexOveload, void (long), overloadedMethod, double (int));
        //QU_OTHER_OVERLOADSLOT(object, other_complexOveloadStrict, void (long), overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(object, other_complexOveloadConst, void (long), overloadedMethod, double (int), prototype::ConstMethod);

        //QU_OVERLOADSLOT(object, complexOveload2, overloadedMethod, double (int, double));
        //QU_OVERLOADSLOT(object, complexOveloadStrict2, overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OVERLOADSLOT(object, complexOveloadConst2, overloadedMethod, double (int), prototype::ConstMethod);

        //QU_OTHER_OVERLOADSLOT(object, other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
        //QU_OTHER_OVERLOADSLOT(object, other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(object, other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), prototype::ConstMethod);

        ASSERT_THROWSM("Binding NULL member",
                prototype::Slot<int (int)> nullSlot(object, (TestClass::ConstMethodType)NULL),
                prototype::BadSlotFunctionPointer*);
        ASSERT_THROWSM("Binding nullptr member",
                prototype::Slot<int (int)> nullSlot2(object, (TestClass::ConstMethodType)nullptr),
                prototype::BadSlotFunctionPointer*);

        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot((TestClass*)NULL, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
        ASSERT_THROWSM("Binding nullptr instance",
                prototype::Slot<int ()> nullSlot((TestClass*)nullptr, &TestClass::notOverloadedMethod),
                prototype::BadSlotInstancePointer*);
    }

    void staticClassSlotTest() {
        QU_STATIC_SLOT(staticSLot, TestClass::notOverloadedMethodSt);
        QU_STATIC_SLOT(staticSLotPtr, &TestClass::notOverloadedMethodSt);

        QU_STATIC_OTHERSLOT(other_staticSLot, void (), TestClass::notOverloadedMethodSt);
        QU_STATIC_OTHERSLOT(other_staticSLotPtr, double (), &TestClass::notOverloadedMethodSt);

        QU_STATIC_OVERLOADSLOT(overload1, TestClass::overloadSt, float (int));
        QU_STATIC_OTHERSLOT(other_overload1, float (int), TestClass::overloadSt);
        QU_STATIC_OVERLOADSLOT(overload1Ptr, &TestClass::overloadSt, float (int));
        QU_STATIC_OTHERSLOT(other_overload1Ptr, float (int), &TestClass::overloadSt);

        QU_STATIC_OVERLOADSLOT(overload2, TestClass::overloadSt, float (int, int));
        QU_STATIC_OTHERSLOT(other_overload2, float (int, int), TestClass::overloadSt);
        QU_STATIC_OVERLOADSLOT(overload2Ptr, &TestClass::overloadSt, float (int, int));
        QU_STATIC_OTHERSLOT(other_overload2Ptr, float (int, int), &TestClass::overloadSt);

        QU_STATIC_OTHER_OVERLOADSLOT(overload1_other, void (float), TestClass::overloadSt, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload1Ptr_other, float (float), &TestClass::overloadSt, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload2_other, void (float ,double), TestClass::overloadSt, float (int, int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload2Ptr_other, int (int, int), &TestClass::overloadSt, float (int, int));


        prototype::Slot<int (int)> staticSlotNULL((FooType)NULL);
        prototype::Slot<int (int)> staticSlotNullPtr((FooType)nullptr);
    }

    void copyConstructorTest() {
        using namespace prototype;
        Slot<int (int)> thisSlot(instance.get(), &TestClass::notOverloadedMethodConst);

        Slot<int (int)> copySlot(thisSlot);
        Slot<void (int)> voidCopySlot(thisSlot);
    }

    void assignTest() {
        using namespace prototype;
        Functor foo;

        Slot<int (int)> thisSlot(instance.get(), &TestClass::notOverloadedMethodConst);
        QU_STATIC_OTHER_OVERLOADSLOT(staticSlot, int (int), &TestClass::overloadSt, float (int));
        Slot<int (int)> functorSlot(foo);

        thisSlot = staticSlot;
        staticSlot = functorSlot;
        functorSlot = thisSlot;

        Slot<void (int)> voidSlot(instance.get(), &TestClass::notOverloadedMethodConst);
        voidSlot = thisSlot;
    }

    void invokeTest() {
        int res = nonEmptyInstanceSlot(5);
        ASSERT_EQUAL(5, res);
        res = deletedSlot(6);
        ASSERT_EQUAL(6, res);

        tempPtr.reset();
        ASSERT_THROWS(deletedSlot(7), prototype::EmptySlot*);
        ASSERT_THROWS(emptySlot(), prototype::EmptySlot*);
    }

    void boolOperatorTest() {
        ASSERT_EQUALM("Not empty slot", true, bool(nonEmptyInstanceSlot));
        ASSERT_EQUALM("Empty slot", false, bool(emptySlot));
        ASSERT_EQUALM("Not deleted instance slot", true, bool(deletedSlot));
        tempPtr.reset();
        ASSERT_EQUALM("Not deleted instance slot", false, bool(deletedSlot));
    }
};


template<class Runner>
void runConstructorTests(Runner& runner) {
    cute::suite s;

    //TODO copy constructor test
    s += CUTE_SMEMFUN(TestSuite, defaultConstructorTest);
    s += CUTE_SMEMFUN(TestClass, thisConstructorTest);
    s += CUTE_SMEMFUN(TestClass, thisConstructorTestConst);
    s += CUTE_SMEMFUN(TestSuite, nonThisConstructorTest);
    s += CUTE_SMEMFUN(TestSuite, nonThisConstructorTestConst);
    s += CUTE_SMEMFUN(TestClass, staticScopeSlotTest);
    s += CUTE_SMEMFUN(TestSuite, staticSlotTest);
    s += CUTE_SMEMFUN(TestSuite, staticClassSlotTest);
    s += CUTE_SMEMFUN(TestSuite, functorConstructorTest);
    s += CUTE_SMEMFUN(TestSuite, weakPointerConstructor);
    s += CUTE_SMEMFUN(TestSuite, copyConstructorTest);

    runner(s, "Signal constructor test");
}

void runSuite(){
    cute::ide_listener lis;
    auto runner = cute::makeRunner(lis);
	runConstructorTests(runner);

	cute::suite s;
    s += CUTE_SMEMFUN(TestSuite, assignTest);
    s += CUTE_SMEMFUN(TestSuite, invokeTest);
    s += CUTE_SMEMFUN(TestSuite, boolOperatorTest);
    runner(s, "SignalTest");
}

int main(){
    runSuite();
    return 0;
}



