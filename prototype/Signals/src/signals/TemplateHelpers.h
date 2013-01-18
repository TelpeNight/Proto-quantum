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


}


#endif /* TEMPLATEHELPERS_H_ */
