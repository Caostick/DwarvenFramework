#include <DwarfPhysics/Physics.h>

#include "BtPhysicsWorld.h"

#include <DwarvenCore/New.h>

auto df::CreatePhysicsWorld()->PhysicsWorld* {
	return new BtPhysicsWorld;
}