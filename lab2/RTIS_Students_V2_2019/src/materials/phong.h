#ifndef PHONG_H
#define PHONG_H

#include "material.h"

class Phong : public Material
{
public:
	Phong();
	Phong(Vector3D kd_, Vector3D ks_, int nShininess_);

	bool hasSpecular() const override;
	bool hasTransmission() const override;
	bool hasDiffuseOrGlossy() const override;
	Vector3D Phong::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const override;
	virtual Vector3D Phong::getDiffuseCoefficient() const override;


private:
	Vector3D ks;
	Vector3D kd;
	int nShininess;
};


#endif // PHONG_H

