/*
 * GameObject.cpp
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#include "GameObject.h"

GameObject::GameObject() {
	// TODO Auto-generated constructor stub

}

GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}

std::shared_ptr<ObjectID> GameObject::getId()
{
	return objId;
}

