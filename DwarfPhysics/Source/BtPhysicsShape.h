#pragma once

#include <DwarfPhysics/PhysicsShape.h>

#include <bullet/btBulletDynamicsCommon.h>

namespace df {
    class BtPhysicsShape : public PhysicsShape {
    public:
        ~BtPhysicsShape() override;

        void SetupCollisionSphere(float radius) override;
        void SetupCollisionBox(const Vec3& halfExtents) override;
        void SetupCollisionCylinder(const Vec3& halfExtents) override;
        void SetupCollisionCapsule(float radius, float height) override;
        void SetupCollisionCone(float radius, float height) override;
        void SetupCollisionRamp(const Vec3& halfExtents) override;

        auto GetShape() const ->btConvexShape*;

    private:
        btConvexShape* m_Shape = nullptr;
    };
}