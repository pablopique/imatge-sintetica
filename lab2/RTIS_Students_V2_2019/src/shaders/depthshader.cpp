#include "depthshader.h"
#include "../core/utils.h"

DepthShader::DepthShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
	Shader(bgColor_), maxDist(maxDist_), color(hitColor_)
{ }

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection closestIntersection;
	if (Utils::getClosestIntersection(r, objList, closestIntersection)) {
		Vector3D distance = closestIntersection.itsPoint - r.o;
		double depthDistance = sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2));
		if (depthDistance < maxDist) {
			return color * (1.0 - depthDistance/maxDist);
		}
		else {
			return bgColor;
		}
	}

	return bgColor;
}