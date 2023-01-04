#pragma once 

#include <DwarfPhysics/PhysicsCharacter.h>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace df {
    class BtPhysicsCharacter : public PhysicsCharcter {
    public:
        BtPhysicsCharacter(btDiscreteDynamicsWorld& world);
        ~BtPhysicsCharacter() override;

        void SetupPosition(const Vec3& position) override;
        void SetupOrienattion(const Quat& rotation) override;
        void SetupOrienattion(const Mat3& rotation) override;
        void SetupTransform(const Transform& transform) override;
        void Build(const PhysicsShape* shape) override;

        void SetTransform(const Transform& transform) override;
        auto GetTransform()->Transform override;

        void SetVelocity(const Vec3& velocity) override;

    private:
        btDiscreteDynamicsWorld& m_World;

        btTransform m_Transform;
        btDefaultMotionState m_MotionState;

        btPairCachingGhostObject* m_GhostObject;
        btKinematicCharacterController* m_Body;
    };
}