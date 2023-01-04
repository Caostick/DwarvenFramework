#pragma once

#include <DwarfPhysics/PhysicsWorld.h>

#include "BtPhysicsBody.h"
#include "BtPhysicsShape.h"

#include <DwarvenCore/ObjectPool.h>

class btDiscreteDynamicsWorld;
class btGhostPairCallback;

namespace df {
	class BtPhysicsWorld : public PhysicsWorld {
	public:
		BtPhysicsWorld();
		~BtPhysicsWorld();

		void Update(float deltaTime) override;

		void SetGravity(float gravity) override;

		auto CreateShape()->PhysicsShape* override;
		void DestroyShape(PhysicsShape* shape) override;

		auto CreateBody()->PhysicsBody* override;
		void DestroyBody(PhysicsBody* body) override;

		auto GetBroadphaseInterface() -> btBroadphaseInterface*;

	private:
		btGhostPairCallback* m_GhostPairCallback = nullptr;
		btDefaultCollisionConfiguration* m_CollisionConfiguration = nullptr;
		btCollisionDispatcher* m_CollisionDispatcher = nullptr;
		btBroadphaseInterface* m_OverlappingPairCache = nullptr;
		btSequentialImpulseConstraintSolver* m_Solver = nullptr;
		btDiscreteDynamicsWorld* m_World;

		ObjectPool<BtPhysicsBody> m_Bodies;
		ObjectPool<BtPhysicsShape> m_Shapes;
	};
}