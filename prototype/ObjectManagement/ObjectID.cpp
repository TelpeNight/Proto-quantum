/*
 * ObjectID.cpp
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#include "ObjectID.h"

int ObjectID::currentId_ = 0;

ObjectID::ObjectID() {
	//get new numeric identifier and assign it to the instance
	currentId_ ++;
	objectId_ = currentId_;


}

ObjectID::~ObjectID() {
	// TODO Auto-generated destructor stub
}

int ObjectID::getId() {
	return objectId_;
}

