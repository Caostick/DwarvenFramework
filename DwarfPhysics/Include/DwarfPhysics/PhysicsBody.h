#pragma once

#include <DwarvenCore/Math/Math.h>

namespace df {
    class PhysicsBody {
    public:
        virtual ~PhysicsBody() {}

        virtual void SetupMass(float mass) = 0;
        virtual void SetupPosition(const Vec3& position) = 0;
        virtual void SetupOrienattion(const Quat& rotation) = 0;
        virtual void SetupOrienattion(const Mat3& rotation) = 0;
        virtual void SetupTransform(const Transform& transform) = 0;
        virtual void SetupCollisionSphere(float radius) = 0;
        virtual void SetupCollisionBox(const Vec3& halfExtents) = 0;
        virtual void SetupCollisionCylinder(const Vec3& halfExtents) = 0;
        virtual void SetupCollisionCapsule(float radius, float height) = 0;
        virtual void SetupCollisionCone(float radius, float height) = 0;
        virtual void SetupCollisionPlane(const Vec3& point, const Vec3& normal) = 0;
        virtual void SetupCollisionRamp(const Vec3& halfExtents)  = 0;
        virtual void Build() = 0;

        virtual void SetTransform(const Transform& transform) = 0;
        virtual auto GetTransform()->Transform = 0;
    };
}