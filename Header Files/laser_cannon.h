#ifndef LASER_CANNON_H
#define LASER_CANNON_H

#include "weapon.h"
#include "laser.h"

class LaserCannon: public Weapon
{
public:
	LaserCannon();
	~LaserCannon();

	virtual void Initialize(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngine, unsigned int parentID = -1);
	virtual void Update(const Ogre::FrameEvent &fe);
	
	virtual void cleanup(PhysicsEngine &physicsEngine);
	virtual void detachFrom(PhysicsEngine &physicsEngine);

	void fire();
	virtual void upgrade(int maxCooldown, int dmg);

private:
	int duration;
	Laser laser;
};

#endif // LASER_CANNON_H