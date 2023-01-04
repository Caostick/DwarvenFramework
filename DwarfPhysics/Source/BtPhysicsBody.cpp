#include "BtPhysicsBody.h"
#include "BtPhysicsShape.h"
#include "TypeConversions.h"

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>

df::BtPhysicsBody::BtPhysicsBody(btDiscreteDynamicsWorld& world)
	: m_World(world)
	, m_Mass(0.0f)
	, m_Body(nullptr) {
}

df::BtPhysicsBody::~BtPhysicsBody() {
	if(m_Body) {
		m_World.removeRigidBody(m_Body);
		delete m_Body;
	}
}

void df::BtPhysicsBody::SetupMass(float mass) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	m_Mass = mass;
}

void df::BtPhysicsBody::SetupPosition(const Vec3& position) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	const auto pos = DfToBt(position);
	m_MotionState.m_startWorldTrans.setOrigin(pos);
	m_MotionState.m_graphicsWorldTrans.setOrigin(pos);
}

void df::BtPhysicsBody::SetupOrienattion(const Quat& rotation) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	const auto rot = DfToBt(rotation);
	m_MotionState.m_startWorldTrans.setRotation(rot);
	m_MotionState.m_graphicsWorldTrans.setRotation(rot);
}

void df::BtPhysicsBody::SetupOrienattion(const Mat3& rotation) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	const auto rot = DfToBt(rotation);
	m_MotionState.m_startWorldTrans.setBasis(rot);
	m_MotionState.m_graphicsWorldTrans.setBasis(rot);
}

void df::BtPhysicsBody::SetupTransform(const Transform& transform) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	SetupPosition(transform.GetPosition());
	SetupOrienattion(transform.GetOrientation());
}

void df::BtPhysicsBody::Build(const PhysicsShape* shape) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");
	DFAssert(shape != nullptr, "Collision shape wasn't created!");

	btConvexShape* btShape = static_cast<const BtPhysicsShape*>(shape)->GetShape();

	btVector3 localInertia(0.0f, 0.0f, 0.0f);

	const bool isDynamic = (m_Mass != 0.f);
	if(isDynamic) {
		btShape->calculateLocalInertia(m_Mass, localInertia);
	}

	const auto rbInfo = btRigidBody::btRigidBodyConstructionInfo(m_Mass, &m_MotionState, btShape, localInertia);
	m_Body = new btRigidBody(rbInfo);
	m_Body->setFriction(0.5f);
	m_Body->setRestitution(0.0f);

	m_World.addRigidBody(m_Body);
}

void df::BtPhysicsBody::SetTransform(const Transform& transform) {
	DFAssert(m_Body != nullptr, "PhysicsBody wasn't built!");

	m_Body->setWorldTransform(DfToBt(transform));
}

auto df::BtPhysicsBody::GetTransform()->Transform {
	DFAssert(m_Body != nullptr, "PhysicsBody wasn't built!");

	btTransform transform;
	m_MotionState.getWorldTransform(transform);

	return BtToDf(transform);
}