#pragma once

#include <DwarvenCore/Math/Math.h>

namespace df {
    class PhysicsShape;
}

namespace df {
    class PhysicsCharacter {
    public:
        virtual ~PhysicsCharacter() {}

        virtual void SetupPosition(const Vec3& position) = 0;
        virtual void SetupOrienattion(const Quat& rotation) = 0;
        virtual void SetupOrienattion(const Mat3& rotation) = 0;
        virtual void SetupTransform(const Transform& transform) = 0;
        virtual void Build(const PhysicsShape* shape) = 0;

        virtual void SetTransform(const Transform& transform) = 0;
        virtual auto GetTransform()->Transform = 0;

        virtual void SetVelocity(const Vec3& velocity)  = 0;
    };
}