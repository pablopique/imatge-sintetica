#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader(double maxDist_, Vector3D bgColor_) :
	Shader(bgColor_), maxDist(maxDist_)
{ }

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Vector3D point;
	Intersection closestIntersection;
	if (Utils::getClosestIntersection(r, objList, closestIntersection)) {
		point = closestIntersection.itsPoint;
		Vector3D finalColor;
		Vector3D wo = -r.d;
		for (int ls = 0; ls < lsList.size(); ls++) {
			Vector3D wi = lsList[ls].getPosition() - point;
			double d = wi.length();
			wi = wi.normalized();
			if (dot(wi, closestIntersection.normal) >= 0) {
				Ray shadowRay(point,wi);
				shadowRay.maxT = d;
				if (!Utils::hasIntersection(shadowRay, objList)) {
					finalColor += Utils::multiplyPerCanal(lsList[ls].getIntensity(point), closestIntersection.shape->getMaterial().getReflectance(closestIntersection.normal, wo, wi));
				}

			}
		}
			
		return finalColor;
	}

	return bgColor;
}