/*
 * ObjectIterator.cpp
 *
 *  Created on: 05.03.2013
 *      Author: gluk
 */

#include <algorithm>
#include <iostream>
#include "ObjectIterator.h"

namespace prototype {

ObjectIterator::ObjectIterator(std::list<ObjPointer>& list, std::string subsystem): _list(list) {
	_subsystemId = subsystem;
	_iterator = _list.begin();
}

ObjectIterator::~ObjectIterator() {
}

bool ObjectIterator::hasNext() {
	if (_iterator == _list.end())
		return false;
	if(!(*_iterator)->containsSubsystemComponent(_subsystemId)) {
		next();
	}
	return (_iterator != _list.end());
}


ObjPointer ObjectIterator::getItem() {
	ObjPointer p = *_iterator;
	int id = (*_iterator)->getId().objecttId; //DEBUG
	next();
	return p;
}

void ObjectIterator::next() {
	for (++_iterator; _iterator != _list.end(); ++_iterator) {
		if((*_iterator)->containsSubsystemComponent(_subsystemId))
			break;
	}
}

}
