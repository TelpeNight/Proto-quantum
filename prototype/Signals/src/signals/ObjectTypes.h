/*
 * ObjectTypes.h
 *
 *  Created on: 19.01.2013
 *      Author: night
 */

#ifndef OBJECTTYPES_H_
#define OBJECTTYPES_H_

namespace prototype {

template<class T>
struct TypeHelper {
    typedef T type;
};

}

#define QU_THIS_TYPE prototype::TypeHelper<std::remove_reference<decltype(*this)>::type>::type

#define QU_OBJ_TYPE(object)\
    prototype::TypeHelper<std::remove_reference<decltype(*object)>::type>::type


#endif /* OBJECTTYPES_H_ */
