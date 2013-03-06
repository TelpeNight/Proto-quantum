/*
 * ObjectIterator.h
 *
 *  Created on: 05.03.2013
 *      Author: gluk
 */

#ifndef OBJECTITERATOR_H_
#define OBJECTITERATOR_H_

#include <list>
#include "object_types.h"

namespace prototype {

/**
 * Iterator provides to subsystem the ability to iterate through game objects which
 * holds components of this subsystem
 */
class ObjectIterator {
public:
	/**
	 * Create an iterator, which moves through game objects with components of subsystem.
	 * list - the list of game objects to iterate
	 */
	ObjectIterator(std::list<ObjPointer>& list, std::string subsystem);
	virtual ~ObjectIterator();

	/**
	 * Checks if there are objects to iterate through
	 */
	bool hasNext();
	/**
	 * Returns item that iterator points to
	 */
	ObjPointer getItem();

private:
	std::list<ObjPointer>& _list;
	std::list<ObjPointer>::iterator _iterator;	//current iterator position
	std::string _subsystemId;

	void next();


};

}
#endif
