/*
 * type_traits.h
 *
 *  Created on: 21.02.2013
 *      Author: night
 */

#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

#include <type_traits>

namespace prototype {
    template<typename _From, typename _To,
        bool = std::__or_<std::is_void<_From>, std::is_function<_To>,
                     std::is_array<_To>>::value>
    struct __is_convertible_helper
    { static constexpr bool value = std::is_void<_To>::value; };

    template<typename _From, typename _To>
    class __is_convertible_helper<_From, _To, false>
    : public std::__sfinae_types
    {
      template<typename _From1, typename _To1>
        static _To __test_aux(_From1&& _from);

      template<typename _From1, typename _To1>
        static decltype(__test_aux<_To1>(std::declval<_From1>()), __one())
    __test(int);

      template<typename, typename>
        static __two __test(...);

    public:
      static constexpr bool value = sizeof(__test<_From, _To>(0)) == 1;
    };

    template<typename _From, typename _To>
        struct is_convertible
        : public std::integral_constant<bool,
                       __is_convertible_helper<_From, _To>::value>
        {};
}



#endif /* TYPE_TRAITS_H_ */
