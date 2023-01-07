#include "BtPhysicsWorld.h"
#include "TypeConversions.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

df::BtPhysicsWorld::BtPhysicsWorld() {
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();
	m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfiguration);

#if 1
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	m_OverlappingPairCache = new btAxisSweep3(worldMin, worldMax);
#else
	m_OverlappingPairCache = new btDbvtBroadphase();
#endif

	m_Solver = new btSequentialImpulseConstraintSolver;

	m_GhostPairCallback = new btGhostPairCallback;
	m_OverlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(m_GhostPairCallback);

	m_World = new btDiscreteDynamicsWorld(
		m_CollisionDispatcher,
		m_OverlappingPairCache,
		m_Solver,
		m_CollisionConfiguration
	);

	m_World->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	m_World->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
}

df::BtPhysicsWorld::~BtPhysicsWorld() {
	delete m_World;
	delete m_Solver;
	delete m_OverlappingPairCache;
	delete m_GhostPairCallback;
	delete m_CollisionDispatcher;
	delete m_CollisionConfiguration;
}

void df::BtPhysicsWorld::Update(float deltaTime) {
	m_World->stepSimulation(deltaTime, 0);
}

void df::BtPhysicsWorld::SetGravity(float gravity) {
	m_World->setGravity(btVector3(0.0f, gravity, 0.0f));
}

auto df::BtPhysicsWorld::CreateShape()->PhysicsShape* {
	return m_Shapes.Create();
}

void df::BtPhysicsWorld::DestroyShape(PhysicsShape* shape) {
	m_Shapes.Destroy(static_cast<BtPhysicsShape*>(shape));
}

auto df::BtPhysicsWorld::CreateBody()->PhysicsBody* {
	return m_Bodies.Create(*m_World);
}

void df::BtPhysicsWorld::DestroyBody(PhysicsBody* body) {
	m_Bodies.Destroy(static_cast<BtPhysicsBody*>(body));
}

auto df::BtPhysicsWorld::CreateCharacter()->PhysicsCharacter* {
	return m_Characters.Create(*m_World);
}

void df::BtPhysicsWorld::DestroyCharacter(PhysicsCharacter* character) {
	m_Characters.Destroy(static_cast<BtPhysicsCharacter*>(character));
}

auto df::BtPhysicsWorld::GetBroadphaseInterface()->btBroadphaseInterface* {
	return m_OverlappingPairCache;
}