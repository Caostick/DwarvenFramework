#include "TypeConversions.h"

auto BtToDf(const btVector3& vector)->Vec3 {
	return Vec3(vector.x(), vector.y(), vector.z());
}

auto BtToDf(const btQuaternion& quat)->Quat {
	return Quat(quat.x(), quat.y(), quat.z(), quat.w());
}

auto BtToDf(const btMatrix3x3& matrix)->Mat3 {
	const auto col0 = matrix.getColumn(0);
	const auto col1 = matrix.getColumn(1);
	const auto col2 = matrix.getColumn(2);
	return Mat3(
		col0.x(), col1.x(), col2.x(),
		col0.y(), col1.y(), col2.y(),
		col0.z(), col1.z(), col2.z()
	);
}

auto BtToDf(const btTransform& transform)->Transform {
	const auto& origin = transform.getOrigin();
	const auto& basis = transform.getBasis();

	return Transform(
		BtToDf(origin),
		BtToDf(basis)
	);
}

auto DfToBt(const Vec3& vector)->btVector3 {
	return btVector3(vector.X, vector.Y, vector.Z);
}

auto DfToBt(const Quat& quat)->btQuaternion {
	return btQuaternion(quat.X, quat.Y, quat.Z, quat.W);
}

auto DfToBt(const Mat3& matrix)->btMatrix3x3 {
	return btMatrix3x3(
		matrix.M[0], matrix.M[1], matrix.M[2],
		matrix.M[3], matrix.M[4], matrix.M[5],
		matrix.M[6], matrix.M[7], matrix.M[8]
	);
}

auto DfToBt(const Transform& transform)->btTransform {
	return btTransform(
		DfToBt(transform.GetOrientation()),
		DfToBt(transform.GetPosition())
	);
}