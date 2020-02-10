#include "transmissive.h"

Transmissive::Transmissive() {};

Transmissive::Transmissive(double medium_, Vector3D v_) :
	medium(medium_), v(v_)
{}

bool Transmissive::hasSpecular() const
{
	return false;
}

bool Transmissive::hasTransmission() const
{
	return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}

Vector3D Transmissive::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const {
	return n * 2 * dot(wo, n) - wo;
}
