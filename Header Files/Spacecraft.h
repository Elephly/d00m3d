#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <OgreVector3.h>
#include "physics_entity.h"
#include "input_manager.h"
#include "laser_cannon.h"

class Spacecraft : public PhysicsEntity
{
public:
	Spacecraft();
	virtual ~Spacecraft() = 0;
	virtual void cleanup(PhysicsEngine &physicsEngine);
	virtual void Initialize(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngine, Ogre::Vector3 lightPos, unsigned int parentID = -1);
	virtual void Update(const Ogre::FrameEvent &fe);

	virtual void Collide(const Ogre::FrameEvent &fe, PhysicsEntity *physicsEntity);

	virtual void spaghettify();

	void fireLaser();

	void ThrustersForward();
	void ThrustersLeft();
	void ThrustersBackward();
	void ThrustersRight();
	void ThrustersUpward();
	void ThrustersDownward();

protected:
	Ogre::String materialPrefix;
	Ogre::Vector3 light;
	Ogre::SceneNode *leftPanelPivot;
	Ogre::SceneNode *rightPanelPivot;
	float thrusterForce;
	Weapon *weapon;
};

#endif // SPACECRAFT_H
