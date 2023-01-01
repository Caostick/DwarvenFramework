#include <DwarfPhysics/Physics.h>
#include <DwarfPhysics/PhysicsWorld.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <DwarvenCore/New.h>

df::Physics::Physics() {
	m_CollisionConfiguration = DFNew btDefaultCollisionConfiguration();
	m_CollisionDispatcher = DFNew btCollisionDispatcher(m_CollisionConfiguration);
	m_OverlappingPairCache = DFNew btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver;
}

df::Physics::~Physics() {
	delete m_Solver;
	DFDelete m_OverlappingPairCache;
	DFDelete m_CollisionDispatcher;
	DFDelete m_CollisionConfiguration;
}

auto df::Physics::CreatePhysicsWorld()->PhysicsWorld* {
	return DFNew PhysicsWorld(new btDiscreteDynamicsWorld(
		m_CollisionDispatcher,
		m_OverlappingPairCache,
		m_Solver,
		m_CollisionConfiguration
	));
}

void df::Physics::DestroyPhysicsWorld(PhysicsWorld* world) {
	DFDelete world;
}