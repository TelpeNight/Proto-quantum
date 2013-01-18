/*
 * ISlot.h
 *
 *  Created on: 14.12.2012
 *      Author: night
 */

#ifndef SLOT_H_
#define SLOT_H_

#include "Signature.h"
#include "TemplateHelpers.h"

#include <functional>
#include <stdexcept>

namespace prototype {

class BadSlotFunctionPointer: public std::invalid_argument {
public:
    BadSlotFunctionPointer(const std::string& what = "") :
            std::invalid_argument(what) {
    }
};

class BadSlotInstancePointer: public std::invalid_argument {
public:
    BadSlotInstancePointer(const std::string& what = "") :
            std::invalid_argument(what) {
    }
};

template<typename FunctionSignature>
class Slot;

template<typename ReturnType, typename ... ArgTypes>
class Slot<ReturnType(ArgTypes...)> {
public:
    typedef ReturnType (FunctionSignature)(ArgTypes...);

    Slot() {}
    Slot(std::nullptr_t) {}

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
    > Slot(Functor&& functor);

    template<typename StaticFunction,
        EnableIf <
            std::is_function<
                RemoveAll<StaticFunction>
            >
        > = {}
    > Slot(StaticFunction&& functor);
    Slot(FunctionSignature* function);

    Slot(const Slot& other);
    Slot(Slot&& other);
    Slot& operator=(const Slot&);
    Slot& operator=(Slot&&);

    ReturnType operator()(ArgTypes&&... arguments);
    ReturnType invoke(ArgTypes&&... arguments);

    bool operator==(const Slot& other);
    bool operator!=(const Slot& other);
    operator bool();

private:
    std::function<FunctionSignature> _function;
};

template<typename Signature>
bool operator==(std::nullptr_t, const Slot<Signature>& slot) {
    return !slot;
}

template<typename Signature>
bool operator==(const Slot<Signature>& slot,std::nullptr_t) {
    return !slot;
}

template<typename Signature>
bool operator!=(std::nullptr_t, const Slot<Signature>& slot) {
    return slot;
}

template<typename Signature>
bool operator!=(const Slot<Signature>& slot,std::nullptr_t) {
    return slot;
}

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

    if (pComponent == nullptr) {
        throw new BadSlotInstancePointer("Binding null instance pointer");
    }
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

    if (pComponent == nullptr) {
        throw new BadSlotInstancePointer("Binding null instance pointer");
    }
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
> Slot<ReturnType(ArgTypes...)>::Slot(StaticFunction&& functor) :
    _function(std::forward<StaticFunction>(functor))
{}

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
> Slot<ReturnType(ArgTypes...)>::Slot(Functor&& functor) :
    _function(std::forward<Functor>(functor))
{}

template<typename ReturnType, typename ... ArgTypes>
ReturnType Slot<ReturnType(ArgTypes...)>::invoke(ArgTypes&&... arguments) {
    return _function(std::forward<ArgTypes>(arguments)...);
}

template<typename ReturnType, typename ... ArgTypes>
ReturnType Slot<ReturnType(ArgTypes...)>::operator()(ArgTypes&&... arguments) {
    return invoke(std::forward<ArgTypes>(arguments)...);
}

} /* namespace prototype */

#endif /* SLOT_H_ */
