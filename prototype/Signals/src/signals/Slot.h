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
#include <type_traits>
#include <assert.h>

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

class EmptySlot: public std::logic_error {
public:
    EmptySlot(const std::string& what = "") :
        std::logic_error(what) {
    }
};

template<typename FunctionSignature>
class Slot;

namespace SlotInternal {
    struct SlotTag{};
    enum class Type : char {NON, Object, Static, Member};
}

template<typename ReturnType, typename ... ArgTypes>
class Slot<ReturnType(ArgTypes...)> {
public:
    typedef SlotInternal::SlotTag Tag;
    typedef ReturnType (FunctionSignature)(ArgTypes...);

    struct TrueFunctor {
        bool operator()() {
            return true;
        }
    };

    constexpr Slot() {}
    constexpr Slot(std::nullptr_t) {}

    template<typename ComponentType, typename MemberType>
    Slot(ComponentType* pComponent, MemberType&& functor);

    template<typename ComponentType>
    Slot(ComponentType* pComponent, ReturnType (ComponentType::*functor)(ArgTypes...));

    //----------------Two functor constructors-------------------//
    template<
        typename Functor,
        EnableIf <
            std::is_class<
                RemoveRef<Functor>
            >
        > = 0,
        DisableIf<
            std::is_same<
                typename RemoveRef<Functor>::SlotTag,
                SlotInternal::SlotTag
            >
        > = 0
    > Slot(Functor&& functor);

    template<
        typename Functor,
        EnableIf <
            std::is_class<
                RemoveRef<Functor>
            >
        > = 0,
        DisableIf<
            HasTag<
                RemoveRef<Functor>
            >
        > = 0
    > Slot(Functor&& functor);
    //-----------------------------------------------------------//

    template<
        typename StaticFunction,
        EnableIf <
            std::is_function<
                RemoveAll<StaticFunction>
            >
        > = 0
    >Slot(StaticFunction&& functor);

    Slot(FunctionSignature* function);

    template <
        class WeakPtrType,
        typename MemberType,
        EnableIf <
            std::is_member_function_pointer<
                decltype(&WeakPtrType::expired)
            >
        > = 0
    >Slot(WeakPtrType& weakPtr, MemberType&& method);

    template <
        class WeakPtrType,
        EnableIf <
            std::is_member_function_pointer<
                decltype(&WeakPtrType::expired)
            >
        > = 0
    >
    Slot(WeakPtrType& weakPtr, ReturnType (WeakPtrType::element_type::*method)(ArgTypes...));

    template <
        class SharedPtrType,
        typename MemberType,
        EnableIf <
            std::is_member_function_pointer<
                decltype(&SharedPtrType::get)
            >
        > = 0
    >Slot(SharedPtrType& sharedPtr, MemberType&& method);

    template <
        class SharedPtrType,
        EnableIf <
            std::is_member_function_pointer<
                decltype(&SharedPtrType::get)
            >
        > = 0
    >Slot(SharedPtrType& sharedPtr, ReturnType (SharedPtrType::element_type::*method)(ArgTypes...));

    template <
        class OtherSlot,
        EnableIf <
            std::is_same <
                typename OtherSlot::Tag,
                SlotInternal::SlotTag
            >
        > = 0,
        DisableIf<
            std::is_same<
                RemoveRef<OtherSlot>,
                Slot<ReturnType(ArgTypes...)>
            >
        > = 0
    >Slot(const OtherSlot& other);

    Slot(const Slot& other);
    Slot(Slot&& other);
    Slot& operator=(const Slot&);
    Slot& operator=(Slot&&);

    ReturnType operator()(ArgTypes&&... arguments);
    ReturnType invoke(ArgTypes&&... arguments);

    //bool operator==(const Slot& other) const;
    //bool operator!=(const Slot& other) const;
    explicit operator bool() const;

//private:
    typedef std::pair<void*,void*> comparisonType;

    std::function<FunctionSignature> _function;
    std::function<bool ()> _statusFunctor = TrueFunctor();
    SlotInternal::Type _type = SlotInternal::Type::NON;
    comparisonType _comparison;

    template<typename OtherSlot>
    bool _equals(const OtherSlot& other) const {
        if (!bool(*this) && !bool(other)) {
            return true;
        }

        if (_type != other._type) {
            return false;
        }
        assert(_type != SlotInternal::Type::NON);

        //TODO provide object comparison
        if (_type == SlotInternal::Type::Object) {
            return false;
        }

        return _comparison.first == other._comparison.first
               && _comparison.second == other._comparison.second;
    }
};

template<typename Signature>
bool operator==(std::nullptr_t, const Slot<Signature>& slot) {
    return !bool(slot);
}

template<typename Signature>
bool operator==(const Slot<Signature>& slot,std::nullptr_t) {
    return !bool(slot);
}

template<typename Signature>
bool operator!=(std::nullptr_t, const Slot<Signature>& slot) {
    return bool(slot);
}

template<typename Signature>
bool operator!=(const Slot<Signature>& slot,std::nullptr_t) {
    return bool(slot);
}

template<typename Signature1, typename Signature2>
bool operator==(const Slot<Signature1>& slot1, const Slot<Signature2>& slot2) {
    return slot1._equals(slot2);
}

template<typename Signature1, typename Signature2>
bool operator!=(const Slot<Signature1>& slot1, const Slot<Signature2>& slot2) {
    return !(slot1 == slot2);
}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::Slot(const Slot& other) :
        _function(other._function),
        _statusFunctor(other._statusFunctor),
        _type(other._type),
        _comparison(other._comparison){}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::Slot(Slot&& other) :
        _function(std::move(other._function)),
        _statusFunctor(std::move(other._statusFunctor)),
        _type(other._type),
        _comparison(std::move(other._comparison)){}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>&
Slot<ReturnType(ArgTypes...)>::operator=(const Slot& other) {
    _function = other._function;
    _type= other._type;
    _statusFunctor = other._statusFunctor;
    _comparison = other._comparison;
    return *this;
}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>&
Slot<ReturnType(ArgTypes...)>::operator=(Slot&& other) {
    _function = std::move(other._function);
    _statusFunctor = std::move(other._statusFunctor);
    _type = other._type;
    _comparison = std::move(other._comparison);
    return *this;
}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::Slot(FunctionSignature* function) :
    _function(function),
    _type(SlotInternal::Type::Static),
    _comparison((void*)NULL, (void*)function){}

template<typename ReturnType, typename ... ArgTypes>
template<typename ComponentType, typename MemberType>
Slot<ReturnType(ArgTypes...)>::Slot(ComponentType* pComponent, MemberType&& functor) :
        _comparison((void*)pComponent,
                     reinterpret_cast<void*>(*reinterpret_cast<int*>(&functor))){
    _type = SlotInternal::Type::Member;

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
                                    ReturnType (ComponentType::*functor)(ArgTypes...)) :
        _comparison((void*)pComponent,
                     reinterpret_cast<void*>(*reinterpret_cast<int*>(&functor))){
    _type = SlotInternal::Type::Member;

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
template <
    class WeakPtrType,
    typename MemberType,
    EnableIf <
        std::is_member_function_pointer<
            decltype(&WeakPtrType::expired)
        >
    >
>
Slot<ReturnType(ArgTypes...)>::Slot(WeakPtrType& weakPtr, MemberType&& method) :
        _comparison((void*)weakPtr.lock().get(),
                 reinterpret_cast<void*>(*reinterpret_cast<int*>(&method))) {
    _type = SlotInternal::Type::Member;

    if (weakPtr.expired()) {
        throw new BadSlotInstancePointer("Binding null instance pointer");  //XXX bad!!! should constructs empty slot instead!
    }
    if (method == nullptr) {
        throw new BadSlotFunctionPointer("Binding null method pointer");
    }

    _function = [weakPtr, method](ArgTypes&&... args) -> ReturnType {
        auto sharedPtr = weakPtr.lock();
        if (!sharedPtr) {
            throw new EmptySlot();
        }
        return static_cast<ReturnType>(
                (sharedPtr.get()->*method)(std::forward<ArgTypes>(args)...)
        );
    };

    _statusFunctor = [weakPtr]() -> bool {
        return !weakPtr.expired();
    };
}

template<typename ReturnType, typename ... ArgTypes>
template <
    class WeakPtrType,
    EnableIf <
        std::is_member_function_pointer<
            decltype(&WeakPtrType::expired)
        >
    >
>
Slot<ReturnType(ArgTypes...)>::Slot(WeakPtrType& weakPtr,
                        ReturnType (WeakPtrType::element_type::*method)(ArgTypes...)) :
        _comparison((void*)weakPtr.lock().get(),
                 reinterpret_cast<void*>(*reinterpret_cast<int*>(&method))) {
    _type = SlotInternal::Type::Member;

    if (weakPtr.expired()) {
        throw new BadSlotInstancePointer("Binding null instance pointer");
    }
    if (method == nullptr) {
        throw new BadSlotFunctionPointer("Binding null method pointer");
    }

    _function = [weakPtr, method](ArgTypes&&... args) -> ReturnType {
        auto sharedPtr = weakPtr.lock();
        if (!sharedPtr) {
            throw new BadSlotInstancePointer("Binding null instance pointer");
        }
        return static_cast<ReturnType>(
                (sharedPtr.get()->*method)(std::forward<ArgTypes>(args)...)
        );
    };

    _statusFunctor = [weakPtr]() -> bool {
        return !weakPtr.expired();
    };
}

template<typename ReturnType, typename ... ArgTypes>
template <
    class SharedPtrType,
    typename MemberType,
    EnableIf <
        std::is_member_function_pointer<
            decltype(&SharedPtrType::get)
        >
    >
>
Slot<ReturnType(ArgTypes...)>::Slot(SharedPtrType& sharedPtr, MemberType&& method) :
        _comparison((void*)sharedPtr.get(),
                 reinterpret_cast<void*>(*reinterpret_cast<int*>(&method))) {
    _type = SlotInternal::Type::Member;

    if (!sharedPtr) {
        throw new BadSlotInstancePointer("Binding null instance pointer");
    }
    if (method == nullptr) {
        throw new BadSlotFunctionPointer("Binding null method pointer");
    }

    _function = [sharedPtr, method](ArgTypes&&... args) -> ReturnType {
        return static_cast<ReturnType>(
                (sharedPtr.get()->*method)(std::forward<ArgTypes>(args)...)
        );
    };
}

template<typename ReturnType, typename ... ArgTypes>
template <
    class SharedPtrType,
    EnableIf <
        std::is_member_function_pointer<
            decltype(&SharedPtrType::get)
        >
    >
>
Slot<ReturnType(ArgTypes...)>::Slot(SharedPtrType& sharedPtr,
                        ReturnType (SharedPtrType::element_type::*method)(ArgTypes...)) :
        _comparison((void*)sharedPtr.get(),
                 reinterpret_cast<void*>(*reinterpret_cast<int*>(&method))) {
    _type = SlotInternal::Type::Member;

    if (!sharedPtr) {
        throw new BadSlotInstancePointer("Binding null instance pointer");
    }
    if (method == nullptr) {
        throw new BadSlotFunctionPointer("Binding null method pointer");
    }

    _function = [sharedPtr, method](ArgTypes&&... args) -> ReturnType {
        return static_cast<ReturnType>(
                (sharedPtr.get()->*method)(std::forward<ArgTypes>(args)...)
        );
    };
}

template<typename ReturnType, typename ... ArgTypes>
template<typename StaticFunction,
    EnableIf<
        std::is_function<RemoveAll<StaticFunction> >
    >
> Slot<ReturnType(ArgTypes...)>::Slot(StaticFunction&& functor) :
    _function(std::forward<StaticFunction>(functor)),
    _type(SlotInternal::Type::Static),
    _comparison(NULL, (void*)functor)
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
            typename RemoveRef<Functor>::SlotTag,
            SlotInternal::SlotTag
        >
    >
> Slot<ReturnType(ArgTypes...)>::Slot(Functor&& functor) :
    _function(std::forward<Functor>(functor)),
    _type(SlotInternal::Type::Object)
{}

template<typename ReturnType, typename ... ArgTypes>
template<typename Functor,
    EnableIf <
        std::is_class<
            RemoveRef<Functor>
        >
    >,
    DisableIf<
        HasTag<
            RemoveRef<Functor>
        >
    >
> Slot<ReturnType(ArgTypes...)>::Slot(Functor&& functor) :
    _function(std::forward<Functor>(functor)),
    _type(SlotInternal::Type::Object)
{}

template<typename ReturnType, typename ... ArgTypes>
ReturnType Slot<ReturnType(ArgTypes...)>::invoke(ArgTypes&&... arguments) {
    if (!_function) {
        throw new EmptySlot("Slot<ReturnType(ArgTypes...)>::invoke");
    }
    return _function(std::forward<ArgTypes>(arguments)...);
}

template<typename ReturnType, typename ... ArgTypes>
ReturnType Slot<ReturnType(ArgTypes...)>::operator()(ArgTypes&&... arguments) {
    return invoke(std::forward<ArgTypes>(arguments)...);
}

template<typename ReturnType, typename ... ArgTypes>
Slot<ReturnType(ArgTypes...)>::operator bool() const {
    return _function != nullptr && _statusFunctor();
}

template<typename ReturnType, typename ... ArgTypes>
template <
    class OtherSlot,
    EnableIf <
        std::is_same <
            typename OtherSlot::Tag,
            SlotInternal::SlotTag
        >
    >,
    DisableIf<
        std::is_same<
            RemoveRef<OtherSlot>,
            Slot<ReturnType(ArgTypes...)>
        >
    >
>Slot<ReturnType (ArgTypes...)>::Slot(const OtherSlot& other) {
    _function = other._function;
    _statusFunctor = other._statusFunctor;
    _type = other._type;
}

} /* namespace prototype */

#endif /* SLOT_H_ */
