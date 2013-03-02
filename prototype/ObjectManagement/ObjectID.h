/*
 * ObjectID.h
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#ifndef OBJECTID_H_
#define OBJECTID_H_

/**
 * This class represents a set of information that identifies an object.
 * Now GameObject is identified by unic integer ID
 */
class ObjectID {

public:
	ObjectID();
	virtual ~ObjectID();

	/**
	 * returns numerical id of current object
	 */
	int getId();

private:
	static int currentId_;
	int objectId_;
};

#endif /* OBJECTID_H_ */
