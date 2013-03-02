/*
 * ObjectID.cpp
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#include "ObjectID.h"

namespace prototype {

int ObjectID::_currentId = 0;

ObjectID::ObjectID() {
	//get new numeric identifier and assign it to the instance
	_currentId ++;
	_objectId = _currentId;


}

ObjectID::~ObjectID() {
	// TODO Auto-generated destructor stub
}

int ObjectID::getId() {
	return _objectId;
}

}
