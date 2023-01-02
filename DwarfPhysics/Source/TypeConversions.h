#pragma once

#include <DwarvenCore/Math/Math.h>

#include <bullet/btBulletDynamicsCommon.h>

auto BtToDf(const btVector3& vector)->Vec3;
auto BtToDf(const btQuaternion& quat)->Quat;
auto BtToDf(const btMatrix3x3& matrix)->Mat3;
auto BtToDf(const btTransform& transform)->Transform;

auto DfToBt(const Vec3& vector)->btVector3;
auto DfToBt(const Quat& quat)->btQuaternion;
auto DfToBt(const Mat3& matrix)->btMatrix3x3;
auto DfToBt(const Transform& transform)->btTransform;