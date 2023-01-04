#include "BtPhysicsShape.h"
#include "TypeConversions.h"

#include <DwarvenCore/Assert.h>

df::BtPhysicsShape::~BtPhysicsShape() {
	if(m_Shape) {
		delete m_Shape;
	}
}

void df::BtPhysicsShape::SetupCollisionSphere(float radius) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btSphereShape(radius);
}

void df::BtPhysicsShape::SetupCollisionBox(const Vec3& halfExtents) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btBoxShape(DfToBt(halfExtents));
}

void df::BtPhysicsShape::SetupCollisionCylinder(const Vec3& halfExtents) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btCylinderShape(DfToBt(halfExtents));
}

void df::BtPhysicsShape::SetupCollisionCapsule(float radius, float height) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btCapsuleShape(radius, height);
}

void df::BtPhysicsShape::SetupCollisionCone(float radius, float height) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	m_Shape = new btConeShape(radius, height);
}

void df::BtPhysicsShape::SetupCollisionRamp(const Vec3& halfExtents) {
	DFAssert(m_Shape == nullptr, "Collision shape is already created!");

	auto shape = new btConvexHullShape();
	m_Shape = shape;

	shape->addPoint(btVector3(-halfExtents.X, -halfExtents.Y, -halfExtents.Z));
	shape->addPoint(btVector3(halfExtents.X, -halfExtents.Y, -halfExtents.Z));
	shape->addPoint(btVector3(-halfExtents.X, -halfExtents.Y, halfExtents.Z));
	shape->addPoint(btVector3(halfExtents.X, -halfExtents.Y, halfExtents.Z));
	shape->addPoint(btVector3(-halfExtents.X, halfExtents.Y, halfExtents.Z));
	shape->addPoint(btVector3(halfExtents.X, halfExtents.Y, halfExtents.Z));
}

auto df::BtPhysicsShape::GetShape() const->btConvexShape* {
	DFAssert(m_Shape != nullptr, "Collision shape wasn't created!");

	return m_Shape;
}