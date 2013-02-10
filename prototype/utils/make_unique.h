/*
 * make_unique.h
 *
 *  Created on: 10.02.2013
 *      Author: night
 */

#ifndef MAKE_UNIQUE_H_
#define MAKE_UNIQUE_H_

#include <memory>

namespace std
{

template<class Class, typename ... ArgTypes>
inline
unique_ptr<Class> make_unique(ArgTypes&&... args) {
    return unique_ptr<Class>(
            new Class(std::forward<ArgTypes>(args)...)
    );
}

}


#endif /* MAKE_UNIQUE_H_ */
