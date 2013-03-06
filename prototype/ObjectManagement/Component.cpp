/*
 * Component.cpp
 *
 *  Created on: 04.03.2013
 *      Author: gluk
 */

#include "Component.h"

namespace prototype {

Component::Component(std::string subsystemId) {
	//_subsystemId = subsystemId;
	_id.subsystemStrId = subsystemId;
}

Component::~Component() {

}

const ComponentId& Component::getId() {
	return _id;
}

} /* namespace Prototype */
