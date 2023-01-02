#pragma once

#include <DwarfPhysics/PhysicsBody.h>

#include <bullet/btBulletDynamicsCommon.h>

namespace df {
	class BtPhysicsBody : public PhysicsBody {
	public:
		BtPhysicsBody(btDiscreteDynamicsWorld& world);
		~BtPhysicsBody() override;

		void SetupMass(float mass) override;
		void SetupPosition(float x, float y, float z) override;
		void SetupPosition(const Vec3& position) override;
		void SetupRotation(const Quat& rotation) override;
		void SetupCollisionSphere(float radius) override;
		void SetupCollisionBox(const Vec3& halfExtents) override;

		void Build() override;
	private:
		btDiscreteDynamicsWorld& m_World;

		btScalar m_Mass;
		btTransform m_Transform;
		btDefaultMotionState m_MotionState;

		btCollisionShape* m_Shape;
		btRigidBody* m_Body;
	};
}