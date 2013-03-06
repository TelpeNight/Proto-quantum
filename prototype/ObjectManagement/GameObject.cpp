/*
 * GameObject.cpp
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#include <algorithm>
#include "GameObject.h"

namespace prototype {

int GameObject::_currentId = 0;

GameObject::GameObject() {
	_currentId++;
	objId.objecttId = _currentId;
}

GameObject::~GameObject() {
}

const ObjectId& GameObject::getId()
{
	return objId;
}

void GameObject::addComponent(Component &comp) {
	components.push_back(comp);
}


bool GameObject::containsSubsystemComponent(std::string subsystem) {
	//TODO change the implementation
	std::list<Component>::iterator it;
	for (it=components.begin(); it!=components.end(); ++it) {
		if ((*it).getId().subsystemStrId == subsystem)
			return true;
	}
	return false;
}

}

