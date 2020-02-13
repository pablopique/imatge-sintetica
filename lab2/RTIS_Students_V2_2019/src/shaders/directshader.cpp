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
		if (closestIntersection.shape->getMaterial().hasTransmission()) {
			Vector3D normalRefraction = closestIntersection.normal.normalized();
			double cosThetaT_out;
			double cosThetaI = dot(normalRefraction, -r.d);
			double eta = closestIntersection.shape->getMaterial().getIndexOfRefraction();
			if (cosThetaI < 0){
				normalRefraction = -normalRefraction;
				cosThetaI = dot(normalRefraction, -r.d);
				eta = 1 / eta;
			}
			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT_out)){
				Vector3D wt = Utils::computeTransmissionDirection(r, normalRefraction, eta, cosThetaI, cosThetaT_out);
				Ray refractRay = Ray(point, wt, r.depth + 1);
				return computeColor(refractRay, objList, lsList);
			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(r.d, closestIntersection.normal.normalized());
				Ray reflectionRay = Ray(point, wr.normalized(), r.depth + 1);
				return computeColor(reflectionRay, objList, lsList);
			}
		}
		if (closestIntersection.shape->getMaterial().hasSpecular()) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, closestIntersection.normal.normalized());
			Ray reflectionRay = Ray(point, wr.normalized(), r.depth + 1);
			return computeColor(reflectionRay, objList, lsList);
		}
		for (int ls = 0; ls < lsList.size(); ls++) {
			Vector3D wi = lsList[ls].getPosition() - point;
			double d = wi.length();
			wi = wi.normalized();
			if (dot(wi, closestIntersection.normal) >= 0) {
				Ray shadowRay(point,wi);
				shadowRay.maxT = d;
				if (!Utils::hasIntersection(shadowRay, objList)) {
					if (closestIntersection.shape->getMaterial().hasDiffuseOrGlossy()) {
						finalColor += Utils::multiplyPerCanal(lsList[ls].getIntensity(point), closestIntersection.shape->getMaterial().getReflectance(closestIntersection.normal, wo, wi));
					}
				}

			}
		}
			
		return finalColor;
	}

	return bgColor;
}