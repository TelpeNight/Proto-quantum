/*
 * object_types.h
 *
 *  Created on: 05.03.2013
 *      Author: gluk
 */

#ifndef OBJECT_TYPES_H_
#define OBJECT_TYPES_H_

#include <memory>
#include <GameObject.h>

namespace prototype {

/**
 * This file defines some names for short identification
 */

typedef std::shared_ptr<GameObject> ObjPointer;
typedef std::weak_ptr<GameObject> weakObjPointer;

}


#endif /* OBJECT_TYPES_H_ */
