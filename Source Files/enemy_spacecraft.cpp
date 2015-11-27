#include "enemy_spacecraft.h"

EnemySpacecraft::EnemySpacecraft(): physicsEngine(NULL), currState(STATE_IDLE), target(NULL), lastShot(0.0), reload(2.0)
{
}

EnemySpacecraft::EnemySpacecraft(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngine, unsigned int parentID):
	physicsEngine(NULL), currState(STATE_IDLE), target(NULL), lastShot(0.0), reload(2.0)
{
	Initialize(sceneManager, parentNode, physicsEngine, parentID);
}

EnemySpacecraft::~EnemySpacecraft()
{
}

void EnemySpacecraft::Initialize(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngineRef, unsigned int parentID){
	Spacecraft::Initialize(sceneManager, parentNode, physicsEngineRef, parentID);
	physicsEngine = &physicsEngineRef;
}

void EnemySpacecraft::Update(const Ogre::FrameEvent &fe)
{
	Spacecraft::Update(fe);
	if(target != NULL){ //if we have a target do stuff
		switch(currState){
		case STATE_IDLE:
			handleIdle(fe);
			break;
		case STATE_PURSUE:
			handlePursue(fe);
			break;
		case STATE_TURN:
			handleTurn(fe);
			break;
		case STATE_FIRE:
			handlePursue(fe);
			handleFire(fe, false);
			break;
		case STATE_WARN:
			handlePursue(fe);
			handleFire(fe, true);
			break;
		default:
			currState = STATE_IDLE;
			break;

		}
	}
}

void EnemySpacecraft::setTarget(Spacecraft* newTarget)
{
	target = newTarget;
}

void EnemySpacecraft::handleIdle(const Ogre::FrameEvent &fe)
{
	//need to orientate to the target.
	findTarget(fe);
	Ogre::Vector3 distance = target->getPosition() - sceneNode->getPosition();
	std::cout << "Distance: " <<distance.length() <<std::endl;
	if(distance.length() < 50){
		currState = STATE_PURSUE;
	}
}

void EnemySpacecraft::handlePursue(const Ogre::FrameEvent &fe)
{
	Ogre::Vector3 distance = target->getPosition() - sceneNode->getPosition();
	std::cout << "Distance: " <<distance.length() <<std::endl;
	if(laser.getState() != Laser::LASER_FIRING){ //ensure we dont turn as the laser is firing
		findTarget(fe);
	}
	if(distance.length() < 10){
		currState = STATE_WARN;
		if(velocity.length() > target->GetVelocity().length()){
			std::cout << "Speed: " << velocity.length() << std::endl;
			ThrustersBackward();
		}

	}else{
		if(currState != STATE_PURSUE){
			currState = STATE_PURSUE;
		}
		ThrustersForward();
	}
	
}

void EnemySpacecraft::handleTurn(const Ogre::FrameEvent &fe)
{
}


void EnemySpacecraft::findTarget(const Ogre::FrameEvent &fe)
{
	sceneNode->lookAt(target->getPosition(), Ogre::Node::TS_PARENT);
}

void EnemySpacecraft::handleFire(const Ogre::FrameEvent &fe, bool warningShot){
	if(lastShot + fe.timeSinceLastFrame > reload){
		lastShot = 0; 
		if(warningShot){
			Ogre::Vector3 targetDirection = target->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
			targetDirection.normalise(); //in case its not normalized;

			sceneNode->lookAt(target->getPosition() + 2*targetDirection, Ogre::Node::TS_PARENT);
			currState = STATE_FIRE;
		}else{
			}
		fireLaser();	
	}else{
		lastShot += fe.timeSinceLastFrame;
	}
	
}
