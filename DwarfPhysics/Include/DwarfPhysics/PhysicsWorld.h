#pragma once

#include <DwarfPhysics/PhysicsBody.h>
#include <DwarfPhysics/PhysicsShape.h>

#include <DwarvenCore/ObjectPool.h>

namespace df {
	class PhysicsWorld {
	public:
		virtual ~PhysicsWorld() {}

		virtual void Update(float deltaTime) = 0;

		virtual void SetGravity(float gravity) = 0;

		virtual auto CreateShape()->PhysicsShape* = 0;
		virtual void DestroyShape(PhysicsShape* shape) = 0;

		virtual auto CreateBody()->PhysicsBody* = 0;
		virtual void DestroyBody(PhysicsBody* body) = 0;
	};
}