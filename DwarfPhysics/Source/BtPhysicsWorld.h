#pragma once

#include <DwarfPhysics/PhysicsWorld.h>

#include "BtPhysicsBody.h"

#include <DwarvenCore/ObjectPool.h>

class btDiscreteDynamicsWorld;

namespace df {
	class BtPhysicsWorld : public PhysicsWorld {
	public:
		BtPhysicsWorld(btDiscreteDynamicsWorld* world);
		~BtPhysicsWorld();

		void Update(float deltaTime) override;

		void SetGravity(float gravity) override;

		auto CreateBody()->PhysicsBody* override;
		void DestroyBody(PhysicsBody* body) override;

	private:
		btDiscreteDynamicsWorld* m_World;

		ObjectPool<BtPhysicsBody> m_Bodies;
	};
}