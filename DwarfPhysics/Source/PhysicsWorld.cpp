#include <DwarfPhysics/PhysicsWorld.h>

#include <bullet/btBulletDynamicsCommon.h>

df::PhysicsWorld::PhysicsWorld(btDiscreteDynamicsWorld* world) 
	: m_World(world) {
	m_World->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

df::PhysicsWorld::~PhysicsWorld() {
	delete m_World;
}

void df::PhysicsWorld::SetGravity(float gravity) {
	m_World->setGravity(btVector3(0.0f, gravity, 0.0f));
}