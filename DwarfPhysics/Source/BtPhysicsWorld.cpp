#include "BtPhysicsWorld.h"

#include <bullet/btBulletDynamicsCommon.h>

df::BtPhysicsWorld::BtPhysicsWorld(btDiscreteDynamicsWorld* world)
	: m_World(world) {
	m_World->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

df::BtPhysicsWorld::~BtPhysicsWorld() {
	delete m_World;
}

void df::BtPhysicsWorld::Update(float deltaTime) {
	m_World->stepSimulation(deltaTime, 10);
}

void df::BtPhysicsWorld::SetGravity(float gravity) {
	m_World->setGravity(btVector3(0.0f, gravity, 0.0f));
}

auto df::BtPhysicsWorld::CreateBody()->PhysicsBody* {
	return m_Bodies.Create(*m_World);
}

void df::BtPhysicsWorld::DestroyBody(PhysicsBody* body) {
	m_Bodies.Destroy(static_cast<BtPhysicsBody*>(body));
}