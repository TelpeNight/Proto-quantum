#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include "signals/Slot.h"
#include "signals/SlotShortcuts.h"
#include "utils/make_unique.h"

#include <memory>
#include <complex>

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
	}

	void sharedPointerConstructor() {

        QU_SLOT(tempPtr, nonConst, notOverloadedMethod);
        QU_SLOT(tempPtr, constSlot, notOverloadedMethodConst);

        QU_OTHERSLOT(tempPtr, other, float (), notOverloadedMethod);
        QU_OTHERSLOT(tempPtr, otherConst, void (double), notOverloadedMethodConst);

        QU_OVERLOADSLOT(tempPtr, overload, overload, float (int));
        QU_OTHERSLOT(tempPtr, overloadOther, float (int), overload);
        QU_OTHER_OVERLOADSLOT(tempPtr, overloadOtherSlot, void (double), overload, float (int));

        QU_OVERLOADSLOT(tempPtr, overload2, overload, float (int, int));
        QU_OTHERSLOT(tempPtr, overloadOther2, float (int, int), overload);
        QU_OTHER_OVERLOADSLOT(tempPtr, overloadOtherSlot2, void (double, float), overload, float (int, int));

        QU_OTHERSLOT(tempPtr, complexOther, double (int), overloadedMethod);
        QU_OTHERSLOT(tempPtr, complexOther2, double (int, double), overloadedMethod);

        QU_OVERLOADSLOT(tempPtr, complexOveload, overloadedMethod, double (int));
        QU_OVERLOADSLOT(tempPtr, complexOveloadStrict, overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OVERLOADSLOT(tempPtr, complexOveloadConst, overloadedMethod, double (int), prototype::ConstMethod);

        QU_OTHER_OVERLOADSLOT(tempPtr, other_complexOveload, void (long), overloadedMethod, double (int));
        QU_OTHER_OVERLOADSLOT(tempPtr, other_complexOveloadStrict, void (long), overloadedMethod, double (int), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(tempPtr, other_complexOveloadConst, void (long), overloadedMethod, double (int), prototype::ConstMethod);

        QU_OVERLOADSLOT(tempPtr, complexOveload2, overloadedMethod, double (int, double));
        QU_OVERLOADSLOT(tempPtr, complexOveloadStrict2, overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OVERLOADSLOT(tempPtr, complexOveloadConst2, overloadedMethod, double (int), prototype::ConstMethod);

        QU_OTHER_OVERLOADSLOT(tempPtr, other_complexOveload2, void (long, long), overloadedMethod, double (int, double));
        QU_OTHER_OVERLOADSLOT(tempPtr, other_complexOveloadStrict2, void (long, float), overloadedMethod, double (int, double), prototype::NonConstMethod);
        QU_OTHER_OVERLOADSLOT(tempPtr, other_complexOveloadConst2, void (long, long), overloadedMethod, double (int, double), prototype::ConstMethod);
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
        voidSlot = nullptr;
    }

    void invokeTest() {
        int res = nonEmptyInstanceSlot(5);
        ASSERT_EQUAL(5, res);
        res = deletedSlot(6);
        ASSERT_EQUAL(6, res);

        tempPtr.reset();
        ASSERT_THROWS(deletedSlot(7), std::bad_function_call*);
        ASSERT_THROWS(emptySlot(), std::bad_function_call*);
    }

    void boolOperatorTest() {
        //XXX bad test coverage
        ASSERT_EQUALM("Not empty slot", true, bool(nonEmptyInstanceSlot));
        ASSERT_EQUALM("Empty slot", false, bool(emptySlot));
        ASSERT_EQUALM("Not deleted instance slot", true, bool(deletedSlot));

        prototype::Slot<int (int)> copiedNotDeletedSlot = deletedSlot;
        ASSERT_EQUALM("Deleted instance slot", true, bool(copiedNotDeletedSlot));

        prototype::Slot<void (int)> copiedNotDeletedSlot2 = deletedSlot;
        ASSERT_EQUALM("Deleted instance slot", true, bool(copiedNotDeletedSlot2));

        tempPtr.reset();
        ASSERT_EQUALM("Deleted instance slot", false, bool(deletedSlot));

        prototype::Slot<int (int)> copiedDeletedSlot = deletedSlot;
        ASSERT_EQUALM("Deleted instance slot", false, bool(copiedDeletedSlot));

        prototype::Slot<void (int)> copiedDeletedSlot2 = deletedSlot;
        ASSERT_EQUALM("Deleted instance slot", false, bool(copiedDeletedSlot2));

        ASSERT_EQUALM("Deleted instance slot", false, bool(copiedNotDeletedSlot));
        ASSERT_EQUALM("Deleted instance slot", false, bool(copiedNotDeletedSlot2));

        nonEmptyInstanceSlot = nullptr;
        ASSERT_EQUALM("Nullified slot", false, bool(nonEmptyInstanceSlot));

        prototype::Slot<int (int)> nullSlot1(instance.get(), (TestClass::MethodType)NULL);
        prototype::Slot<int (int)> nullSlot2(instance.get(), (TestClass::MethodType)nullptr);
        prototype::Slot<int ()> nullSlot3((TestClass*)NULL, &TestClass::notOverloadedMethod);
        prototype::Slot<int ()> nullSlot4((TestClass*)nullptr, &TestClass::notOverloadedMethod);
        prototype::Slot<int ()> nullSlot5(tempPtr, &TestClass::notOverloadedMethod);
        prototype::Slot<int ()> nullSlot6(nullPtr, &TestClass::notOverloadedMethod);
        ASSERT(!bool(nullSlot1));
        ASSERT(!bool(nullSlot2));
        ASSERT(!bool(nullSlot3));
        ASSERT(!bool(nullSlot4));
        ASSERT(!bool(nullSlot5));
        ASSERT(!bool(nullSlot6));
    }

    void nullptrComparisonTest() {
        ASSERT_EQUAL(true, nonEmptyInstanceSlot != nullptr);
        ASSERT_EQUAL(false, nonEmptyInstanceSlot == nullptr);
        ASSERT_EQUAL(false, emptySlot != nullptr);
        ASSERT_EQUAL(true, emptySlot == nullptr);
        ASSERT_EQUAL(true, deletedSlot != nullptr);
        ASSERT_EQUAL(false, deletedSlot == nullptr);

        prototype::Slot<int (int)> copiedNotDeletedSlot = deletedSlot;
        ASSERT_EQUAL(true, copiedNotDeletedSlot != nullptr);
        ASSERT_EQUAL(false, copiedNotDeletedSlot == nullptr);

        prototype::Slot<void (int)> copiedNotDeletedSlot2 = deletedSlot;
        ASSERT_EQUAL(true, copiedNotDeletedSlot2 != nullptr);
        ASSERT_EQUAL(false, copiedNotDeletedSlot2 == nullptr);

        tempPtr.reset();
        ASSERT_EQUAL(false, deletedSlot != nullptr);
        ASSERT_EQUAL(true, deletedSlot == nullptr);

        prototype::Slot<int (int)> copiedDeletedSlot = deletedSlot;
        ASSERT_EQUAL(false, copiedDeletedSlot != nullptr);
        ASSERT_EQUAL(true, copiedDeletedSlot == nullptr);

        prototype::Slot<void (int)> copiedDeletedSlot2 = deletedSlot;
        ASSERT_EQUAL(false, copiedDeletedSlot2 != nullptr);
        ASSERT_EQUAL(true, copiedDeletedSlot2 == nullptr);

        ASSERT_EQUALM("Deleted instance slot", false, copiedNotDeletedSlot != nullptr);
        ASSERT_EQUALM("Deleted instance slot", true, copiedNotDeletedSlot == nullptr);
        ASSERT_EQUALM("Deleted instance slot", false, nullptr != copiedNotDeletedSlot2);
        ASSERT_EQUALM("Deleted instance slot", true, nullptr == copiedNotDeletedSlot2);
    }

    void comparisonTest() {
        using namespace prototype;

        //statics
        QU_STATIC_SLOT(staticSLot, notOverloadedFunction);
        QU_STATIC_SLOT(staticSLotPtr, &notOverloadedFunction);
        QU_STATIC_OVERLOADSLOT(overload1, overloadFunction, float (int));
        QU_STATIC_OVERLOADSLOT(overload1Ptr, &overloadFunction, float (int));
        QU_STATIC_OTHER_OVERLOADSLOT(overload1Sig, void (int), overloadFunction, float (int));
        ASSERT(staticSLot == staticSLotPtr);
        ASSERT_EQUAL(false, staticSLot != staticSLotPtr);
        ASSERT(overload1 == overload1Ptr);
        ASSERT_EQUAL(false, overload1 != overload1Ptr);
        ASSERT(overload1 == overload1Sig);
        ASSERT_EQUAL(false, overload1 != overload1Sig);
        ASSERT(overload1Sig == overload1);
        ASSERT_EQUAL(false, overload1Sig != overload1);
        ASSERT(overload1Ptr == overload1Sig);
        ASSERT_EQUAL(false, overload1Ptr != overload1Sig);
        ASSERT(overload1Sig == overload1Ptr);
        ASSERT_EQUAL(false, overload1Sig != overload1Ptr);
        ASSERT(staticSLot != overload1Ptr);
        ASSERT(overload1 != staticSLotPtr);

        Slot<void (float)> copySlot = overload1;
        ASSERT(copySlot == overload1Ptr);
        ASSERT(overload1 == copySlot);
        ASSERT(copySlot != staticSLotPtr);

        //instance slots
        auto object = tempPtr.get();
        QU_OTHERSLOT(object, complexOther1a, double (int), overloadedMethod);
        QU_OTHERSLOT(object, complexOther2a, double (int, double), overloadedMethod);
        QU_OTHER_OVERLOADSLOT(object, complexOther1b, int (float), overloadedMethod, double (int));
        QU_OTHER_OVERLOADSLOT(object, complexOther2b, double (int, int), overloadedMethod, double (int, double));
        ASSERT(complexOther1a == complexOther1b);
        ASSERT(complexOther1b == complexOther1a);
        ASSERT(complexOther2a == complexOther2b);
        ASSERT(complexOther2b == complexOther2a);
        ASSERT(complexOther2a == complexOther2a);
        ASSERT(complexOther1a != complexOther2b);
        ASSERT(complexOther1b != complexOther2a);
        ASSERT(complexOther2a != complexOther1b);
        ASSERT(complexOther2b != complexOther1a);
        ASSERT(complexOther2a != complexOther1a);
        ASSERT(complexOther2b != complexOther1b);

        ASSERT_EQUAL(false, complexOther1a != complexOther1b);
        ASSERT_EQUAL(false, complexOther1b != complexOther1a);
        ASSERT_EQUAL(false, complexOther2a != complexOther2b);
        ASSERT_EQUAL(false, complexOther2b != complexOther2a);
        ASSERT_EQUAL(false, complexOther2a != complexOther2a);
        ASSERT_EQUAL(false, complexOther1a == complexOther2b);
        ASSERT_EQUAL(false, complexOther1b == complexOther2a);
        ASSERT_EQUAL(false, complexOther2a == complexOther1b);
        ASSERT_EQUAL(false, complexOther2b == complexOther1a);
        ASSERT_EQUAL(false, complexOther2a == complexOther1a);
        ASSERT_EQUAL(false, complexOther2b == complexOther1b);

        {
        //compare shared variant with simple
            QU_OTHER_OVERLOADSLOT(tempPtr, complexOther1bs, int (float), overloadedMethod, double (int));
            QU_OTHER_OVERLOADSLOT(tempPtr, complexOther2bs, double (int, int), overloadedMethod, double (int, double));

            ASSERT(complexOther1a == complexOther1bs);
            ASSERT(complexOther1bs == complexOther1a);
            ASSERT(complexOther2a == complexOther2bs);
            ASSERT(complexOther2bs == complexOther2a);
            ASSERT(complexOther2a == complexOther2a);
            ASSERT(complexOther1a != complexOther2bs);
            ASSERT(complexOther1bs != complexOther2a);
            ASSERT(complexOther2a != complexOther1bs);
            ASSERT(complexOther2bs != complexOther1a);
            ASSERT(complexOther2a != complexOther1a);
            ASSERT(complexOther2bs != complexOther1bs);

            ASSERT_EQUAL(false, complexOther1a != complexOther1bs);
            ASSERT_EQUAL(false, complexOther1bs != complexOther1a);
            ASSERT_EQUAL(false, complexOther2a != complexOther2bs);
            ASSERT_EQUAL(false, complexOther2bs != complexOther2a);
            ASSERT_EQUAL(false, complexOther2a != complexOther2a);
            ASSERT_EQUAL(false, complexOther1a == complexOther2bs);
            ASSERT_EQUAL(false, complexOther1bs == complexOther2a);
            ASSERT_EQUAL(false, complexOther2a == complexOther1bs);
            ASSERT_EQUAL(false, complexOther2bs == complexOther1a);
            ASSERT_EQUAL(false, complexOther2a == complexOther1a);
            ASSERT_EQUAL(false, complexOther2bs == complexOther1bs);
        }

        //compare weak variant with simple
        QU_OTHER_OVERLOADSLOT(ptr, complexOther1bw, int (float), overloadedMethod, double (int));
        QU_OTHER_OVERLOADSLOT(ptr, complexOther2bw, void (int, int), overloadedMethod, double (int, double));

        ASSERT(complexOther1a == complexOther1bw);
        ASSERT(complexOther1bw == complexOther1a);
        ASSERT(complexOther2a == complexOther2bw);
        ASSERT(complexOther2bw == complexOther2a);
        ASSERT(complexOther2a == complexOther2a);
        ASSERT(complexOther1a != complexOther2bw);
        ASSERT(complexOther1bw != complexOther2a);
        ASSERT(complexOther2a != complexOther1bw);
        ASSERT(complexOther2bw != complexOther1a);
        ASSERT(complexOther2a != complexOther1a);
        ASSERT(complexOther2bw != complexOther1bw);

        ASSERT_EQUAL(false, complexOther1a != complexOther1bw);
        ASSERT_EQUAL(false, complexOther1bw != complexOther1a);
        ASSERT_EQUAL(false, complexOther2a != complexOther2bw);
        ASSERT_EQUAL(false, complexOther2bw != complexOther2a);
        ASSERT_EQUAL(false, complexOther2a != complexOther2a);
        ASSERT_EQUAL(false, complexOther1a == complexOther2bw);
        ASSERT_EQUAL(false, complexOther1bw == complexOther2a);
        ASSERT_EQUAL(false, complexOther2a == complexOther1bw);
        ASSERT_EQUAL(false, complexOther2bw == complexOther1a);
        ASSERT_EQUAL(false, complexOther2a == complexOther1a);
        ASSERT_EQUAL(false, complexOther2bw == complexOther1bw);

        //compare weak with shared
        {
            QU_OTHERSLOT(ptr, complexOther1aw, double (int), overloadedMethod);
            QU_OTHERSLOT(ptr, complexOther2aw, double (int, double), overloadedMethod);
            QU_OTHER_OVERLOADSLOT(tempPtr, complexOther1bs, std::complex<float> (float), overloadedMethod, double (int));
            QU_OTHER_OVERLOADSLOT(tempPtr, complexOther2bs, void (int, int), overloadedMethod, double (int, double));

            ASSERT(complexOther1aw == complexOther1bs);
            ASSERT(complexOther1bs == complexOther1aw);
            ASSERT(complexOther2aw == complexOther2bs);
            ASSERT(complexOther2bs == complexOther2aw);
            ASSERT(complexOther2aw == complexOther2aw);
            ASSERT(complexOther1aw != complexOther2bs);
            ASSERT(complexOther1bs != complexOther2aw);
            ASSERT(complexOther2aw != complexOther1bs);
            ASSERT(complexOther2bs != complexOther1aw);
            ASSERT(complexOther2aw != complexOther1aw);
            ASSERT(complexOther2bs != complexOther1bs);

            ASSERT_EQUAL(false, complexOther1aw != complexOther1bs);
            ASSERT_EQUAL(false, complexOther1bs != complexOther1aw);
            ASSERT_EQUAL(false, complexOther2aw != complexOther2bs);
            ASSERT_EQUAL(false, complexOther2bs != complexOther2aw);
            ASSERT_EQUAL(false, complexOther2aw != complexOther2aw);
            ASSERT_EQUAL(false, complexOther1aw == complexOther2bs);
            ASSERT_EQUAL(false, complexOther1bs == complexOther2aw);
            ASSERT_EQUAL(false, complexOther2aw == complexOther1bs);
            ASSERT_EQUAL(false, complexOther2bs == complexOther1aw);
            ASSERT_EQUAL(false, complexOther2aw == complexOther1aw);
            ASSERT_EQUAL(false, complexOther2bs == complexOther1bs);
        }
        //release shared, but keep pointer and compare with weak variant
        tempPtr.reset();
        ASSERT(complexOther1a != complexOther1bw);
        ASSERT(complexOther1bw != complexOther1a);
        ASSERT(complexOther2a != complexOther2bw);
        ASSERT(complexOther2bw != complexOther2a);
        ASSERT(complexOther1a != complexOther2bw);
        ASSERT(complexOther1bw != complexOther2a);
        ASSERT(complexOther2a != complexOther1bw);
        ASSERT(complexOther2bw != complexOther1a);

        ASSERT_EQUAL(false, complexOther1a == complexOther1bw);
        ASSERT_EQUAL(false, complexOther1bw == complexOther1a);
        ASSERT_EQUAL(false, complexOther2a == complexOther2bw);
        ASSERT_EQUAL(false, complexOther2bw == complexOther2a);
        ASSERT_EQUAL(false, complexOther1a == complexOther2bw);
        ASSERT_EQUAL(false, complexOther1bw == complexOther2a);
        ASSERT_EQUAL(false, complexOther2a == complexOther1bw);
        ASSERT_EQUAL(false, complexOther2bw == complexOther1a);

        //empty slots should be equal
        ASSERT(complexOther2bw == complexOther1bw);
        ASSERT(complexOther1bw == complexOther2bw);
        ASSERT(emptySlot == complexOther1bw);
        ASSERT(emptySlot == complexOther2bw);
        ASSERT(complexOther2bw == emptySlot);
        ASSERT(complexOther1bw == emptySlot);

        ASSERT_EQUAL(false, complexOther2bw != complexOther1bw);
        ASSERT_EQUAL(false, complexOther1bw != complexOther2bw);
        ASSERT_EQUAL(false, emptySlot != complexOther1bw);
        ASSERT_EQUAL(false, emptySlot != complexOther2bw);
        ASSERT_EQUAL(false, complexOther2bw != emptySlot);
        ASSERT_EQUAL(false, complexOther1bw != emptySlot);
    }
};


template<class Runner>
void runConstructorTests(Runner& runner) {
    cute::suite s;

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
    s += CUTE_SMEMFUN(TestSuite, nullptrComparisonTest);
    s += CUTE_SMEMFUN(TestSuite, comparisonTest);
    runner(s, "SignalTest");
}

int main(){
    runSuite();
    return 0;
}



