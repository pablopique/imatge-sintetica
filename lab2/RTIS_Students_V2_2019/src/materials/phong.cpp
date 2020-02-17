#include "phong.h"

Phong::Phong() {};

Phong::Phong(Vector3D kd_, Vector3D ks_, int nShininess_) :
	kd(kd_),ks(ks_),nShininess(nShininess_)
{}

bool Phong::hasSpecular() const
{
	// This material does not have specular component
	return false;
}

bool Phong::hasTransmission() const
{
	return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
	return true;
}

Vector3D Phong::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const {
	Vector3D diffuse;
	Vector3D specular;

	Vector3D wr = n * 2 * (dot(n, wi)) - wi;

	diffuse = kd * dot(wi, n);
	specular = ks * (pow(dot(wo, wr), nShininess));

	return diffuse + specular;
}

Vector3D Phong::getDiffuseCoefficient() const {
	return kd;
}