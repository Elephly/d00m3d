#ifndef LASER_H
#define LASER_H

#include "physics_entity.h"
#include <OgreEntity.h>

class Laser: public PhysicsEntity
{
public:
	Laser();
	~Laser();

	enum LASER_STATE{
		LASER_COOLDOWN,
		LASER_READY,
		LASER_FIRING
	};

	LASER_STATE getState();
	virtual void Initialize(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngine);
	void Update(const Ogre::FrameEvent &fe);

	void fire();

	void upgrade(int maxCooldown, int dmg);

	

private:
	int cooldown;
	int duration;
	int timer;
	int damage;
	bool active;

	LASER_STATE myState;
};

#endif //LASER_H