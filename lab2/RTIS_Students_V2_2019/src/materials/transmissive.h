#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_H

#include "material.h"

class Transmissive : public Material
{
public:
	Transmissive();
	Transmissive(double medium, Vector3D v);

	bool hasSpecular() const override;
	bool hasTransmission() const override;
	bool hasDiffuseOrGlossy() const override;
	Vector3D Transmissive::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const override;
	double Transmissive::getIndexOfRefraction() const override;

private:
	Vector3D v;
	double medium;
};


#endif // TRANSMISSIVE_H

