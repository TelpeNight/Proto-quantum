/*
 * GameObject.h
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <list>
#include "Component.h"

namespace prototype {

/**
 *  Structure discribes ways of object identification
 */
struct ObjectId
{
	int objecttId;	//identification by unic numeric id
};

/*
 * Class represents any of objects in game.
 * It works like container for components from different subsystems
 * ObjectManagementSystem is used to create such objects
 */
class GameObject {

public:
	GameObject();
	virtual ~GameObject();
	/**
	 * return reference to object identificator
	 */
	const ObjectId& getId();
	/**
	 * Attaches component comp to this object
	 */
	void addComponent(Component &comp);
	/**
	 * Checks, if this game object contains component of subsystem
	 */
	bool containsSubsystemComponent(std::string subsystem);

private:
	static int _currentId;
	ObjectId objId;
	std::list<Component> components;
};

}

#endif /* GAMEOBJECT_H_ */
