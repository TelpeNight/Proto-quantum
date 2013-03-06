/*
 * ObjectManagementSystem.cpp
 *
 *  Created on: 02.03.2013
 *      Author: gluk
 */

#include <list>
#include <algorithm>
#include "ObjectManagementSystem.h"


namespace prototype {

ObjectManagementSystem::ObjectManagementSystem() {

}

ObjectManagementSystem::~ObjectManagementSystem() {
}

weakObjPointer ObjectManagementSystem::createObject()
{
	ObjPointer newObject = std::make_shared<GameObject>();
	_items.push_back(newObject);
	return newObject;
}

int ObjectManagementSystem::getObjectsCount() {
	return _items.size();
}


weakObjPointer ObjectManagementSystem::getObject(int objId) {
	//TODO change the implementation

	std::list<ObjPointer>::iterator it;
	for (it=_items.begin(); it!=_items.end(); ++it)
	{
		if ((*it)->getId().objecttId == objId)
		{
			return *it;
		}
	}

	return std::weak_ptr<GameObject>();
}

ObjectIterator ObjectManagementSystem::getIterator(std::string subsystem) {
	return ObjectIterator(_items, subsystem);
}

}
