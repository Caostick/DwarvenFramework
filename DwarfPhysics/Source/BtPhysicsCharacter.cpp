#include "BtPhysicsCharacter.h"
#include "BtPhysicsShape.h"
#include "TypeConversions.h"

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>

df::BtPhysicsCharacter::BtPhysicsCharacter(btDiscreteDynamicsWorld& world)
	: m_World(world)
	, m_GhostObject(nullptr)
	, m_Body(nullptr) {}

df::BtPhysicsCharacter::~BtPhysicsCharacter() {
	if(m_Body) {
		m_World.removeCharacter(m_Body);
		delete m_Body;
	}

	if(m_GhostObject) {
		delete m_GhostObject;
	}
}

void df::BtPhysicsCharacter::SetupPosition(const Vec3& position) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	const auto pos = DfToBt(position);
	m_MotionState.m_startWorldTrans.setOrigin(pos);
	m_MotionState.m_graphicsWorldTrans.setOrigin(pos);
}

void df::BtPhysicsCharacter::SetupOrienattion(const Quat& rotation) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	const auto rot = DfToBt(rotation);
	m_MotionState.m_startWorldTrans.setRotation(rot);
	m_MotionState.m_graphicsWorldTrans.setRotation(rot);
}

void df::BtPhysicsCharacter::SetupOrienattion(const Mat3& rotation) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	const auto rot = DfToBt(rotation);
	m_MotionState.m_startWorldTrans.setBasis(rot);
	m_MotionState.m_graphicsWorldTrans.setBasis(rot);
}

void df::BtPhysicsCharacter::SetupTransform(const Transform& transform) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");

	SetupPosition(transform.GetPosition());
	SetupOrienattion(transform.GetOrientation());
}

void df::BtPhysicsCharacter::Build(const PhysicsShape* shape) {
	DFAssert(m_Body == nullptr, "PhysicsBody is alerady built!");
	DFAssert(shape != nullptr, "Collision shape wasn't created!");

	btConvexShape* btShape = static_cast<const BtPhysicsShape*>(shape)->GetShape();

	btVector3 localInertia(0.0f, 0.0f, 0.0f);

	const float mass = 0.5f;
	const bool isDynamic = (mass != 0.f);
	if(isDynamic) {
		btShape->calculateLocalInertia(mass, localInertia);
	}

	m_GhostObject = new btPairCachingGhostObject;
	m_GhostObject->setWorldTransform(m_Transform);
	m_GhostObject->setCollisionShape(btShape);
	m_GhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	const btScalar stepHeight = btScalar(0.35);

	m_Body = new btKinematicCharacterController(m_GhostObject, btShape, stepHeight, btVector3(0.0f, 1.0f, 0.0f));

	m_World.addCharacter(m_Body);
}

void df::BtPhysicsCharacter::SetTransform(const Transform& /*transform*/) {
	DFAssert(m_Body != nullptr, "PhysicsBody wasn't built!");

	//m_Body->set  setWorldTransform(DfToBt(transform));
}

auto df::BtPhysicsCharacter::GetTransform()->Transform {
	DFAssert(m_Body != nullptr, "PhysicsBody wasn't built!");

	btTransform transform;
	m_MotionState.getWorldTransform(transform);

	return BtToDf(transform);
}

void df::BtPhysicsCharacter::SetVelocity(const Vec3& velocity) {
	DFAssert(m_Body != nullptr, "PhysicsBody wasn't built!");

	m_Body->setLinearVelocity(DfToBt(velocity));
}