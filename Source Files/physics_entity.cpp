#include "physics_entity.h"

#include "defines.h"

PhysicsEntity::PhysicsEntity()
{
}

PhysicsEntity::~PhysicsEntity()
{
}
void PhysicsEntity::cleanup(PhysicsEngine &physicsEngine)
{
	detachFrom(physicsEngine);
	Entity::cleanup();
}

void PhysicsEntity::detachFrom(PhysicsEngine &physicsEngine)
{
	physicsEngine.RemovePhysicsEntity((PhysicsEntity*) this);
}

void PhysicsEntity::Initialize(Ogre::SceneManager *sceneManager, Ogre::SceneNode* parentNode, PhysicsEngine &physicsEngine, unsigned int parentID)
{
	Entity::Initialize(sceneManager, parentNode, parentID);
	physicsEngine.AddPhysicsEntity(this);
	absoluteGravitationalPull = false;
	appliedForce = 0.0f;
	bodyType = ENTITY_BODY_SPHERE;
	dynamic = true;
	gravitational = false;
	mass = 1.0f;
	restitution = 0.0f;
	previousVelocity = 0.0f;
	velocity = 0.0f;
	alive = true;
}

void PhysicsEntity::Update(const Ogre::FrameEvent &fe)
{
	Entity::Update(fe);
	if (alive)
	{
		if (dynamic)
		{
			if (mass != 0.0f)
			{
				mass = min(mass, 1000000000000000.0f);
				previousVelocity = velocity;
				velocity += appliedForce / mass;
			}
			translate(velocity * fe.timeSinceLastFrame);
			appliedForce = 0;
		}
	}
}

void PhysicsEntity::ApplyForce(float x, float y, float z)
{
	if (dynamic)
	{
		appliedForce += Ogre::Vector3(x, y, z);
	}
}

void PhysicsEntity::ApplyForce(Ogre::Vector3 force)
{
	if (dynamic)
	{
		appliedForce += force;
	}
}

Ogre::Vector3 PhysicsEntity::GetAppliedForce()
{
	return appliedForce;
}

Ogre::Vector3 PhysicsEntity::GetDeltaVelocity()
{
	return velocity - previousVelocity;
}

Ogre::Vector3 PhysicsEntity::GetPreviousVelocity()
{
	return previousVelocity;
}

Ogre::Vector3 PhysicsEntity::GetVelocity()
{
	return velocity;
}

float PhysicsEntity::GetMass()
{
	return mass;
}

float PhysicsEntity::GetRestitution()
{
	return restitution;
}

float PhysicsEntity::getSpeed()
{
	return velocity.squaredLength();
}

void PhysicsEntity::SetMass(float m)
{
	mass = m;
}

void PhysicsEntity::SetVelocity(Ogre::Vector3 v)
{
	velocity = v;
}

ENTITY_BODY_TYPE PhysicsEntity::GetBodyType()
{
	return bodyType;
}

bool PhysicsEntity::IsAbsoluteGravitationalPull()
{
	return absoluteGravitationalPull;
}

bool PhysicsEntity::IsDynamic()
{
	return dynamic;
}

bool PhysicsEntity::IsGravitational()
{
	return gravitational;
}

void PhysicsEntity::SetBodyType(ENTITY_BODY_TYPE type)
{
	bodyType = type;
}

float PhysicsEntity::GetRadius()
{
	if (sceneNode != 0)
	{
		Ogre::Vector3 scale = sceneNode->getScale();
		return max(scale.x, max(scale.y, scale.z)) / 2.0f;
	}
	return 0.0f;
}