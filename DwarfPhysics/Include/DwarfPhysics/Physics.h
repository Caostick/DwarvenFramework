#pragma once

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;

namespace df {
    class PhysicsWorld;
}

namespace df {
	class Physics {
	public:
		Physics();
		~Physics();

        auto CreatePhysicsWorld() -> PhysicsWorld*;
		void DestroyPhysicsWorld(PhysicsWorld* world);

	private:
		btDefaultCollisionConfiguration* m_CollisionConfiguration = nullptr;
		btCollisionDispatcher* m_CollisionDispatcher = nullptr;
		btBroadphaseInterface* m_OverlappingPairCache = nullptr;
		btSequentialImpulseConstraintSolver* m_Solver = nullptr;
	};
}