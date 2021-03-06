#ifndef STAR_H
#define STAR_H

#include "physics_entity.h"
#include <OgreEntity.h>

class Star: public PhysicsEntity{
public:
	Star(Ogre::SceneManager* sceneManager, Ogre::SceneNode *parentNode, PhysicsEngine &physicsEngine, unsigned int parentID = -1);
	Star();
	~Star();
	void Initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode *parentNode, PhysicsEngine &physicsEngine, unsigned int parentID = -1);
	virtual void Collide(const Ogre::FrameEvent &fe, PhysicsEntity *physicsEntity);
	virtual float GetRadius();
	virtual Resource collect(Entity* entity);

	virtual void spaghettify();
private:
	float calcRadiation(Entity* entity);
};

#endif //STAR_H