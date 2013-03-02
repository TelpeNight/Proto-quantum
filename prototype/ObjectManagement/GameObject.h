/*
 * GameObject.h
 *
 *  Created on: 28.02.2013
 *      Author: gluk
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <memory>
#include <ObjectID.h>


namespace prototype {
/*
 * Class represents any of objects in game.
 * It works like container for components from different subsystems
 * ObjectManagementSystem is used to create such objects
 */
class GameObject {

public:
	GameObject();
	virtual ~GameObject();

	std::shared_ptr<ObjectID> getId();

private:
	std::shared_ptr<ObjectID> objId = std::make_shared<ObjectID>();
};

}

#endif /* GAMEOBJECT_H_ */
