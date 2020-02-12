#include "mirror.h"

Mirror::Mirror() {};

Mirror::Mirror(Vector3D v_) :
	 v(v_)
{}

bool Mirror::hasSpecular() const
{
	return true;
}

bool Mirror::hasTransmission() const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}

Vector3D Mirror::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const {
	return Vector3D(0);
}
