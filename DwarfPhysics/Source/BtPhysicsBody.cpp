#include "BtPhysicsBody.h"

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>

df::BtPhysicsBody::BtPhysicsBody(btDiscreteDynamicsWorld& world)
	: m_World(world)
	, m_Mass(0.0f)
	, m_Shape(nullptr)
	, m_Body(nullptr) {
}

df::BtPhysicsBody::~BtPhysicsBody() {
	if(m_Body) {
		m_World.removeRigidBody(m_Body);
		delete m_Body;
	}

	if(m_Shape) {
		delete m_Shape;
	}
}

void df::BtPhysicsBody::SetupMass(float mass) {
	m_Mass = mass;
}

void df::BtPhysicsBody::SetupPosition(float x, float y, float z) {
	m_MotionState.m_startWorldTrans.setOrigin(btVector3(x, y, z));
	
}

void df::BtPhysicsBody::SetupPosition(const Vec3& position) {
	m_MotionState.m_startWorldTrans.setOrigin(btVector3(position.X, position.Y, position.Y));
}

void df::BtPhysicsBody::SetupRotation(const Quat& rotation) {
	m_MotionState.m_startWorldTrans.setRotation(btQuaternion(rotation.X, rotation.Y, rotation.Z, rotation.W));
}

void df::BtPhysicsBody::SetupCollisionSphere(float radius) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btSphereShape(radius);
}

void df::BtPhysicsBody::SetupCollisionBox(const Vec3& halfExtents) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btBoxShape(btVector3(halfExtents.X, halfExtents.Y, halfExtents.Y));
}

void df::BtPhysicsBody::Build() {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");
	DFAssert(m_Shape != nullptr, "Collision shape wasn't created!");

	btVector3 localInertia(0.0f, 0.0f, 0.0f);

	const bool isDynamic = (m_Mass != 0.f);
	if(isDynamic) {
		m_Shape->calculateLocalInertia(m_Mass, localInertia);
	}

	auto rbInfo = btRigidBody::btRigidBodyConstructionInfo(m_Mass, &m_MotionState, m_Shape, localInertia);
	m_Body = new btRigidBody(rbInfo);

	m_World.addRigidBody(m_Body);
}