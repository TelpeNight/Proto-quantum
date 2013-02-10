/*
 * TemplateHelpers.h
 *
 *  Created on: 19.01.2013
 *      Author: night
 */

#ifndef TEMPLATEHELPERS_H_
#define TEMPLATEHELPERS_H_

#include "ObjectTypes.h"

#include <type_traits>

namespace prototype {

template <typename T>
using Invoke = typename T::type;

namespace detail {enum class enabler {};}

template <typename If, typename Then, typename Else>
using Conditional = Invoke<std::conditional<If::value, Then, Else>>;

template<typename ... T>
struct All: std::true_type {
};

template<typename Head, typename ... Tail>
struct All<Head, Tail...> : public Conditional<Head,
        All<Tail...>, std::false_type> {};

template<typename ... T>
struct One: std::false_type {};

template<typename Head, typename ... Tail>
struct One<Head, Tail...> : public Conditional<Head,
    std::true_type, One<Tail...> > {};

template <typename... Condition>
using EnableIfAll = Invoke<
        std::enable_if<
            All<Condition...>::value,
            detail::enabler
        >
>;

template<typename... Condition>
using DisableIfAll = Invoke<
        std::enable_if<
            !All<Condition...>::value,
            detail::enabler
        >
>;

template <typename... Condition>
using EnableIfOne = Invoke<
        std::enable_if<
            One<Condition...>::value,
            detail::enabler
        >
>;

template<typename... Condition>
using DisableIfOne = Invoke<
        std::enable_if<
            !One<Condition...>::value,
            detail::enabler
        >
>;


template <typename... Condition>
using EnableIf = EnableIfAll<Condition...>;

template <typename... Condition>
using DisableIf = DisableIfOne<Condition...>;

template <typename T>
using RemoveAll =
        Invoke<std::remove_reference<
            Invoke<std::remove_pointer<T> >
        >
>;

template <typename T>
using RemoveRef = Invoke<
        std::remove_reference<T>
>;

template<class T, class R = void>
struct EnableIfType
{
    typedef R type;
};

template<typename T, class Enabler = void>
struct HasTag : std::false_type{};

template<typename T>
struct HasTag<T, typename EnableIfType<typename T::Tag>::type> : std::true_type{};

#define QU_DECLARE_HAS_TAG_CHECKER(__typename) \
        template<typename T>    \
        struct IsType<T, typename EnableIfType<T::__typename>::type> : std::true_type{};

}


#endif /* TEMPLATEHELPERS_H_ */
