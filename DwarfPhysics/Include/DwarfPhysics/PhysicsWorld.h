#pragma once

#include <DwarfPhysics/PhysicsBody.h>

#include <DwarvenCore/ObjectPool.h>

class btDiscreteDynamicsWorld;

namespace df {
	class PhysicsWorld {
	public:
		virtual ~PhysicsWorld() {}

		virtual void Update(float deltaTime) = 0;

		virtual void SetGravity(float gravity) = 0;

		virtual auto CreateBody()->PhysicsBody* = 0;
		virtual void DestroyBody(PhysicsBody* body) = 0;
	};
}