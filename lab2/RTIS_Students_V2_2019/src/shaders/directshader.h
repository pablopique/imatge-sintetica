#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"

class DirectShader : public Shader
{
public:
	DirectShader() = delete;
	DirectShader(double maxDist_, Vector3D bgColor_);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

private:
	double maxDist; 
};

#endif // DIRECTSHADER_H
#pragma once
