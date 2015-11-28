#include "laser_cannon.h"

LaserCannon::LaserCannon(): duration(5)
{
	//lalala
}

LaserCannon::~LaserCannon()
{
}

void LaserCannon::Initialize(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngine, unsigned int parentID)
{
	Weapon::Initialize(sceneManager, parentNode, physicsEngine, parentID);

	unsigned int pid = (parentID == -1 ? objectID : parentID);

	Ogre::Entity *cannonPivotEntity = sceneManager->createEntity("Sphere");
	cannonPivotEntity->setMaterialName("SpacecraftCannonMaterial");
	Ogre::SceneNode *cannonPivotNode = sceneNode->createChildSceneNode("CannonPivot" + Ogre::StringConverter::toString(entityCount));
	cannonPivotNode->attachObject(cannonPivotEntity);
	cannonPivotNode->scale(0.25f, 0.25f, 0.25f);

	Ogre::Entity *cannonBarrelEntity = sceneManager->createEntity("Torus");
	cannonBarrelEntity->setMaterialName("SpacecraftCannonMaterial");
	Ogre::SceneNode *cannonBarrelNode = cannonPivotNode->createChildSceneNode("CannonBarrel" + Ogre::StringConverter::toString(entityCount));
	cannonBarrelNode->attachObject(cannonBarrelEntity);
	cannonBarrelNode->scale(1.0f / 0.25f, 1.0f / 0.25f, 1.0f / 0.25f);
	cannonBarrelNode->scale(0.25f, 0.25f, 1.5f);
	cannonBarrelNode->translate(0.0f, 0.0f, -0.75f);

	laser.Initialize(sceneManager, cannonBarrelNode, physicsEngine, pid);
	laser.scale(1.0f / 0.25f, 1.0f / 0.25f, 1.0f / 1.5f);
	laser.translate(0.0f, 0.0f, 0.3f);
}

void LaserCannon::Update(const Ogre::FrameEvent &fe)
{
	PhysicsEntity::Update(fe);
	if(alive){
		if(timer == 0){
			switch(myState){
			case WEAPON_COOLDOWN:
				myState = WEAPON_READY;
				break;
			case WEAPON_FIRING:
				laser.cooldown();
				myState = WEAPON_COOLDOWN;
				timer = cooldown;
				break;
			}
		}
		else{
			timer--;
		}
	}
}

void LaserCannon::fire()
{
	if(myState == WEAPON_READY){
		myState = WEAPON_FIRING;
		timer = duration;
		laser.fire();
	}
}