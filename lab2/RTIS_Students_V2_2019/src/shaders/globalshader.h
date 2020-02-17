#ifndef GLOBAL_SHADER_H
#define GLOBAL_SHADER_H

#include "shader.h"

class GlobalShader : public Shader
{
public:
	GlobalShader();
	GlobalShader(double maxDist_, Vector3D bgColor_,Vector3D at_);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;
private:
	double maxDist;
	Vector3D at; 

};

#endif // GLOBAL_SHADER_H
