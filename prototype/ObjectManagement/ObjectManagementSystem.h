/*
 * ObjectManagementSystem.h
 *
 *  Created on: 02.03.2013
 *      Author: gluk
 */

#ifndef OBJECTMANAGEMENTSYSTEM_H_
#define OBJECTMANAGEMENTSYSTEM_H_


#include <list>
#include <memory>

#include "ObjectIterator.h"
#include "object_types.h"

namespace prototype {

/*
 *	Class manages all GameObjects.
 *	It holds the list of game objects, manages their livingtime and provides other subsystems
 *	with ability to get access to objects with their components
 */
class ObjectManagementSystem {
public:
	ObjectManagementSystem();
	virtual ~ObjectManagementSystem();

	/*
	 * Creates empty game object and returns pointer to newly created object for further manipulation
	 */
	weakObjPointer createObject();

	/**
	 * Returns the current count of game objects. Now is used for testing, later can be removed
	 */
	int getObjectsCount();

	/**
	 * Returns weak_ptr to GameObject with certain id.
	 */
	weakObjPointer getObject(int objId);

	/**
	 * Create iterator through objects with subsystem components
	 */
	ObjectIterator getIterator(std::string subsystem);

private:
	std::list<ObjPointer> _items;


};


}

#endif
