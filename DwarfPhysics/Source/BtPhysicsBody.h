#pragma once

#include <DwarfPhysics/PhysicsBody.h>

#include <bullet/btBulletDynamicsCommon.h>

namespace df {
	class BtPhysicsBody : public PhysicsBody {
	public:
		BtPhysicsBody(btDiscreteDynamicsWorld& world);
		~BtPhysicsBody() override;

		void SetupMass(float mass) override;
		void SetupPosition(const Vec3& position) override;
		void SetupRotation(const Quat& rotation) override;
		void SetupRotation(const Mat3& rotation) override;
		void SetupTransform(const Transform& transform) override;
		void SetupCollisionSphere(float radius) override;
		void SetupCollisionBox(const Vec3& halfExtents) override;
		void SetupCollisionCylinder(const Vec3& halfExtents) override;
		void SetupCollisionCapsule(float radius, float height) override;
		void Build() override;

		void SetTransform(const Transform& transform) override;
		auto GetTransform()->Transform override;

	private:
		btDiscreteDynamicsWorld& m_World;

		btScalar m_Mass;
		btTransform m_Transform;
		btDefaultMotionState m_MotionState;

		btCollisionShape* m_Shape;
		btRigidBody* m_Body;
	};
}