#pragma once

#include <DwarvenCore/Math/Math.h>

namespace df {
    class PhysicsBody {
    public:
        virtual ~PhysicsBody() {}

        virtual void SetupMass(float mass) = 0;
        virtual void SetupPosition(float x, float y, float z) = 0;
        virtual void SetupPosition(const Vec3& position) = 0;
        virtual void SetupRotation(const Quat& rotation) = 0;
        virtual void SetupCollisionSphere(float radius) = 0;
        virtual void SetupCollisionBox(const Vec3& halfExtents) = 0;
        virtual void Build() = 0;
    };
}