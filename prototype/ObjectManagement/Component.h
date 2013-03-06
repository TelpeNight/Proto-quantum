/*
 * Component.h
 *
 *  Created on: 04.03.2013
 *      Author: gluk
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>

namespace prototype {

/**
 *  Structure discribes different ways of component identification
 */
struct ComponentId {
	std::string subsystemStrId;	//string identificator of subsystem that holds this component
};

/**
 * Class is the base class for GameObject components.
 * Components are special behaviors that are added to game
 * objects and are managed by different game subsystems.
 * All object properties are implemented with the help of
 * components
 */
class Component {
public:
	Component(std::string subsystemId);
	virtual ~Component();

	/**
	 * Returns reference to component identification structure
	 */
	const ComponentId& getId();

private:
	ComponentId _id;
};

}
#endif /* COMPONENT_H_ */
