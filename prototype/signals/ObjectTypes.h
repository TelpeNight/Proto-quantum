/*
 * ObjectTypes.h
 *
 *  Created on: 19.01.2013
 *      Author: night
 */

#ifndef OBJECTTYPES_H_
#define OBJECTTYPES_H_
#include <vector>
namespace prototype {

template <class Pointer>
struct AnyPointerType {
    typedef typename Pointer::element_type type;
};

template <class Class>
struct AnyPointerType<Class*> {
    typedef typename std::remove_reference<Class>::type type;
};

}


#define QU_OBJ_TYPE(object)\
    prototype::AnyPointerType<decltype(object)>::type

#define QU_THIS_TYPE QU_OBJ_TYPE(this)


#endif /* OBJECTTYPES_H_ */
