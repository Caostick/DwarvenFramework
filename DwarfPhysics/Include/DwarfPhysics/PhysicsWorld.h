#pragma once

class btDiscreteDynamicsWorld;

namespace df {
	class PhysicsWorld {
	public:
		PhysicsWorld(btDiscreteDynamicsWorld* world);
		~PhysicsWorld();

		void SetGravity(float gravity);

	private:
		btDiscreteDynamicsWorld* m_World;
	};
}