/*
 * Signature.h
 *
 *  Created on: 19.01.2013
 *      Author: night
 */

#ifndef SIGNATURE_H_
#define SIGNATURE_H_

namespace prototype {

class ConstMethod{};
class NonConstMethod{};

template <class Class, typename Signature, class IsConst = NonConstMethod>
class MemberFunctionCast;

template<class Class, typename ReturnType, typename... Args>
class MemberFunctionCast<Class, ReturnType (Args...), NonConstMethod> {
public:
    typedef ReturnType (Class::*MemberType)(Args...);
    static MemberType cast(MemberType method) {
        return method;
    }
};

template<class Class, typename ReturnType, typename... Args>
class MemberFunctionCast<Class, ReturnType (Args...), ConstMethod> {
public:
    typedef ReturnType (Class::*MemberType)(Args...) const;
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

    template<class Functor>
    static auto cast(Functor&& functor) -> decltype(std::forward<Functor>(functor)) {
        return std::forward<Functor>(functor);
    }
};

template <typename MemberSignature>
class MemberSignatureExtractor;

template <class Class, typename ReturnType, typename... ArgsType>
class MemberSignatureExtractor<ReturnType (Class::*)(ArgsType...)> {
public:
    typedef ReturnType (type)(ArgsType...);
};

template <class Class, typename ReturnType, typename... ArgsType>
class MemberSignatureExtractor<ReturnType (Class::*)(ArgsType...) const> {
public:
    typedef ReturnType (type)(ArgsType...);
};

template <typename FunctionType>
class FunctionSignatureExtractor {
public:
    typedef typename MemberSignatureExtractor<decltype(&FunctionType::operator())>::type type;
};

template <typename ReturnType, typename... ArgsType>
class FunctionSignatureExtractor<ReturnType (*)(ArgsType...)> {
public:
    typedef ReturnType (type)(ArgsType...);
};

template <typename ReturnType, typename... ArgsType>
class FunctionSignatureExtractor<ReturnType (ArgsType...)> {
public:
    typedef ReturnType (type)(ArgsType...);
};

//template <class Functor>
//class FunctionSignatureExtractor<Functor> {
//
//};

}


#endif /* SIGNATURE_H_ */
