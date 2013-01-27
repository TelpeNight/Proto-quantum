/*
 * ObjectTypes.h
 *
 *  Created on: 19.01.2013
 *      Author: night
 */

#ifndef OBJECTTYPES_H_
#define OBJECTTYPES_H_

#define QU_THIS_TYPE std::remove_reference<decltype(*this)>::type

#define QU_OBJ_TYPE(object)\
    std::remove_reference<decltype(*object)>::type


#endif /* OBJECTTYPES_H_ */
