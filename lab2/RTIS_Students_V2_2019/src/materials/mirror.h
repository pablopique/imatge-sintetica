#ifndef MIRROR_H
#define MIRROR_H

#include "material.h"

class Mirror : public Material
{
public:
	Mirror();
	Mirror(Vector3D v);

	bool hasSpecular() const override;
	bool hasTransmission() const override;
	bool hasDiffuseOrGlossy() const override;
	Vector3D Mirror::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const override;

private:
	Vector3D v;
};


#endif // MIRROR_H

