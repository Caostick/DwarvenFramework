#pragma once

#include <DwarvenCore/Math/Math.h>

namespace df {
	class PhysicsShape {
	public:
        virtual ~PhysicsShape() {}

        virtual void SetupCollisionSphere(float radius) = 0;
        virtual void SetupCollisionBox(const Vec3& halfExtents) = 0;
        virtual void SetupCollisionCylinder(const Vec3& halfExtents) = 0;
        virtual void SetupCollisionCapsule(float radius, float height) = 0;
        virtual void SetupCollisionCone(float radius, float height) = 0;
        virtual void SetupCollisionRamp(const Vec3& halfExtents) = 0;
	};
}