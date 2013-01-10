/*
 * ISlot.h
 *
 *  Created on: 14.12.2012
 *      Author: night
 */

#ifndef SLOT_H_
#define SLOT_H_

#include <functional>
#include <cstddef>
#include <type_traits>
#include <stdexcept>
#include <iostream>
#include <utility>

namespace prototype {

template<class T>
struct TypeHelper {
    typedef T type;
};

template <typename T>
using Invoke = typename T::type;

template <typename T>
using Type = Invoke<TypeHelper<T> >;

namespace detail {enum class enabler {};}

template<typename ... T>
struct All: std::true_type {
};

template <typename If, typename Then, typename Else>
using Conditional = Invoke<std::conditional<If::value, Then, Else>>;

template<typename Head, typename ... Tail>
struct All<Head, Tail...> : public Conditional<Head,
        All<Tail...>, std::false_type> {};

template <typename... Condition>
using EnableIf = Invoke<
        std::enable_if<
            All<Condition...>::value,
            detail::enabler
        >
>;

template<typename... Condition>
using DisableIf = Invoke<
        std::enable_if<
            !All<Condition...>::value,
            detail::enabler
        >
>;

template <typename T>
using RemoveAll =
        Invoke<std::remove_reference<
            Invoke<std::remove_pointer<T> >
        >
>;

template <typename T>
using RemoveRef =
        Invoke<std::remove_reference<T>
>;

template <class Class, typename Signature>
class MemberFunctionCast;

template<class Class, typename ReturnType, typename... Args>
class MemberFunctionCast<Class, ReturnType (Args...)> {
public:
    typedef ReturnType (Class::*MemberType)(Args...);
    static MemberType cast(MemberType method) {
        return method;
    }
};

template <typename Signature>
class SignaturePointerCast;

template<typename ReturnType, typename... Args>
class SignaturePointerCast<ReturnType (Args...)> {
public:
    typedef ReturnType (*MemberType)(Args...);
    static MemberType cast(MemberType method) {
        return method;
    }
};

class BadSlotFunctionPointer: public std::invalid_argument {
public:
    BadSlotFunctionPointer(const std::string& what) :
            std::invalid_argument(what) {
    }
};

template<typename FunctionSignature>
class Slot;

template<typename ReturnType, typename ... ArgTypes>
class Slot<ReturnType(ArgTypes...)> {
public:
    typedef ReturnType (FunctionSignature)(ArgTypes...);

    template<typename ComponentType, typename MemberType>
    Slot(ComponentType* pComponent, MemberType&& functor);

    template<typename ComponentType>
    Slot(ComponentType* pComponent, ReturnType (ComponentType::*functor)(ArgTypes...));

    template<typename Functor,
        EnableIf <
            std::is_class<
                RemoveRef<Functor>
            >
        > = {},
        DisableIf<
            std::is_same<
                RemoveRef<Functor>,
                Slot<ReturnType(ArgTypes...)>
            >
        > = {}
    >
    Slot(Functor&& functor);

    template<typename StaticFunction,
        EnableIf <
            std::is_function<
                RemoveAll<StaticFunction>
            >
        > = {}
    >
    Slot(StaticFunction&& functor);
    Slot(FunctionSignature* function);

    Slot(const Slot& other);
    Slot(Slot&& other);
    Slot& operator=(const Slot&);
    Slot& operator=(Slot&&);

    ReturnType operator()(ArgTypes&&... arguments);
    ReturnType invoke(ArgTypes&&... arguments);

private:
    std::function<FunctionSignature> _function;
};

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::Slot(const Slot& other) :
        _function(other._function) {}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::Slot(Slot&& other) :
        _function(std::move(other._function)) {}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>&
Slot<ReturnType(ArgTypes...)>::operator=(const Slot& other) {
    _function = other._function;
    return *this;
}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::Slot(FunctionSignature* function) :
    _function(function){}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>&
Slot<ReturnType(ArgTypes...)>::operator=(Slot&& other) {
    _function = std::move(other._function);
    return *this;
}

template<typename ReturnType, typename ... ArgTypes>
template<typename ComponentType, typename MemberType>
Slot<ReturnType(ArgTypes...)>::Slot(ComponentType* pComponent, MemberType&& functor) {

    if (functor == nullptr) {
        throw new BadSlotFunctionPointer("Binding null method pointer");
	}

	_function = [pComponent, functor](ArgTypes&&... args) -> ReturnType {
		return static_cast<ReturnType>(
				(pComponent->*functor)(std::forward<ArgTypes>(args)...)
		);
	};
}

template<typename ReturnType, typename ... ArgTypes>
template<typename ComponentType>
Slot<ReturnType(ArgTypes...)>::Slot(ComponentType* pComponent,
                                    ReturnType (ComponentType::*functor)(ArgTypes...)) {

    if (functor == nullptr) {
        throw new BadSlotFunctionPointer("Binding null method pointer");
    }

    _function = [pComponent, functor](ArgTypes&&... args) -> ReturnType {
        return static_cast<ReturnType>(
                (pComponent->*functor)(std::forward<ArgTypes>(args)...)
        );
    };
}

template<typename ReturnType, typename ... ArgTypes>
template<typename StaticFunction,
    EnableIf<
        std::is_function<RemoveAll<StaticFunction> >
    >
>
Slot<ReturnType(ArgTypes...)>::Slot(StaticFunction&& functor)
{
    if (functor == nullptr) {
        throw new BadSlotFunctionPointer("Slot with null function pointer");
    }
    _function = std::forward<StaticFunction>(functor);
}

template<typename ReturnType, typename ... ArgTypes>
template<typename Functor,
    EnableIf <
        std::is_class<
            RemoveRef<Functor>
        >
    >,
    DisableIf<
        std::is_same<
            RemoveRef<Functor>,
            Slot<ReturnType(ArgTypes...)>
        >
    >
>
Slot<ReturnType(ArgTypes...)>::Slot(Functor&& functor) :
    _function(std::forward<Functor>(functor))
{}

//TODO invoke functor in specific context. All arguments should by copied
template<typename ReturnType, typename ... ArgTypes>
ReturnType Slot<ReturnType(ArgTypes...)>::invoke(ArgTypes&&... arguments) {
    return _function(std::forward<ArgTypes>(arguments)...);
}

template<typename ReturnType, typename ... ArgTypes>
ReturnType Slot<ReturnType(ArgTypes...)>::operator()(ArgTypes&&... arguments) {
    return invoke(std::forward<ArgTypes>(arguments)...);
}

} /* namespace prototype */

#define QU_THIS_TYPE prototype::TypeHelper<std::remove_reference<decltype(*this)>::type>::type

//TODO test shortcuts
//TODO add additional shortcuts
#define QU_METHOD_RETURN_TYPE(ClassType, methodname, methodArgs) \
		decltype(std::declval< ClassType >().##methodname##(std::declval< methodArgs >()...))

#define QU_METHOD_RETURN_TYPE_2(ClassType, methodname, methodArgs) \
		typename std::result_of<decltype(&FakeComponent::method)(FakeComponent,int)>::type b = 6;

#define QU_THIS_SLOT(varname, method, signature)        \
		prototype::Slot< signature > varname (this, & QU_THIS_TYPE::method)

#define QU_THIS_OVERLOADSLOT(slotSignature, varname, method, signature)        \
        prototype::Slot< slotSignature > varname (this,             \
            MemberFunctionCast<QU_THIS_TYPE, signature>::cast(& QU_THIS_TYPE::method)       \
)

#define QU_THIS_STATICSLOT(slotSignature, varname, method, signature)        \
        prototype::Slot< slotSignature > varname (                           \
            SignaturePointerCast<signature>::cast(& QU_THIS_TYPE::method)       \
)

#define quCallback(method, signature)        \
		prototype::Slot< signature > (this, & QU_THIS_TYPE::method)

#define quOverloadCallback(slotSignature, method, signature)        \
        prototype::Slot< slotSignature > (this,             \
            MemberFunctionCast<QU_THIS_TYPE, signature>::cast(& QU_THIS_TYPE::method)   \
)

#endif /* SLOT_H_ */
