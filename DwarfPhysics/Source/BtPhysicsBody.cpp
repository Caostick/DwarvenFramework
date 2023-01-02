#include "BtPhysicsBody.h"
#include "TypeConversions.h"

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

void df::BtPhysicsBody::SetupPosition(const Vec3& position) {
	const auto pos = DfToBt(position);
	m_MotionState.m_startWorldTrans.setOrigin(pos);
	m_MotionState.m_graphicsWorldTrans.setOrigin(pos);
}

void df::BtPhysicsBody::SetupRotation(const Quat& rotation) {
	const auto rot = DfToBt(rotation);
	m_MotionState.m_startWorldTrans.setRotation(rot);
	m_MotionState.m_graphicsWorldTrans.setRotation(rot);
}

void df::BtPhysicsBody::SetupRotation(const Mat3& rotation) {
	const auto rot = DfToBt(rotation);
	m_MotionState.m_startWorldTrans.setBasis(rot);
	m_MotionState.m_graphicsWorldTrans.setBasis(rot);
}

void df::BtPhysicsBody::SetupTransform(const Transform& transform) {
	SetupPosition(transform.GetPosition());
	SetupRotation(transform.GetOrientation());
}

void df::BtPhysicsBody::SetupCollisionSphere(float radius) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btSphereShape(radius);
}

void df::BtPhysicsBody::SetupCollisionBox(const Vec3& halfExtents) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btBoxShape(DfToBt(halfExtents));
}

void df::BtPhysicsBody::SetupCollisionCylinder(const Vec3& halfExtents) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btCylinderShape(DfToBt(halfExtents));
}

void df::BtPhysicsBody::SetupCollisionCapsule(float radius, float height) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btCapsuleShape(radius, height);
}

void df::BtPhysicsBody::Build() {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");
	DFAssert(m_Shape != nullptr, "Collision shape wasn't created!");

	btVector3 localInertia(0.0f, 0.0f, 0.0f);

	const bool isDynamic = (m_Mass != 0.f);
	if(isDynamic) {
		m_Shape->calculateLocalInertia(m_Mass, localInertia);
	}

	const auto rbInfo = btRigidBody::btRigidBodyConstructionInfo(m_Mass, &m_MotionState, m_Shape, localInertia);
	m_Body = new btRigidBody(rbInfo);

	m_World.addRigidBody(m_Body);
}

void df::BtPhysicsBody::SetTransform(const Transform& transform) {
	m_Body->setWorldTransform(DfToBt(transform));
}

auto df::BtPhysicsBody::GetTransform()->Transform {
	DFAssert(m_Body != nullptr, "PhysicsBody wasn't built!");

	btTransform transform;
	m_MotionState.getWorldTransform(transform);

	return BtToDf(transform);
}