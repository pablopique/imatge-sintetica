#include "globalshader.h"
#include "../core/utils.h"
#include "../sampler/hemisphericalsampler.h"
#include <math.h>

GlobalShader::GlobalShader(double maxDist_, Vector3D bgColor_, Vector3D at_) :
	Shader(bgColor_), maxDist(maxDist_),  at(at_)
{ }
 
Vector3D GlobalShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Vector3D point;
	Intersection closestIntersection;
	if (Utils::getClosestIntersection(r, objList, closestIntersection)) {
		point = closestIntersection.itsPoint;
		Vector3D lDir,lIndir;
		Vector3D wo = -r.d;
		if (closestIntersection.shape->getMaterial().hasTransmission()) {
			Vector3D normalRefraction = closestIntersection.normal.normalized();
			double cosThetaT_out;
			double cosThetaI = dot(normalRefraction, -r.d);
			double eta = closestIntersection.shape->getMaterial().getIndexOfRefraction();
			if (cosThetaI < 0) {
				normalRefraction = -normalRefraction;
				cosThetaI = dot(normalRefraction, -r.d);
				eta = 1 / eta;
			}
			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT_out)) {
				Vector3D wt = Utils::computeTransmissionDirection(r, normalRefraction, eta, cosThetaI, cosThetaT_out);
				Ray refractRay = Ray(point, wt, r.depth);
				return computeColor(refractRay, objList, lsList);
			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(r.d, closestIntersection.normal.normalized());
				Ray reflectionRay = Ray(point, wr.normalized(), r.depth);
				return computeColor(reflectionRay, objList, lsList);
			}
		}
		if (closestIntersection.shape->getMaterial().hasSpecular()) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, closestIntersection.normal.normalized());
			Ray reflectionRay = Ray(point, wr.normalized(), r.depth);
			return computeColor(reflectionRay, objList, lsList);
		}

		if (closestIntersection.shape->getMaterial().hasDiffuseOrGlossy()) {
			HemisphericalSampler Sampler;
			if (r.depth == maxDist) {
				lIndir += Utils::multiplyPerCanal(closestIntersection.shape->getMaterial().getDiffuseCoefficient(), at);
			}else if (r.depth == 0) {
				int nSample = 50;
				for (int i = 0; i < nSample; i++) {
					Vector3D wj = Sampler.getSample(closestIntersection.normal.normalized());
					Ray ray(point, wj.normalized(), r.depth + 1);
					Vector3D dirReflection = closestIntersection.shape->getMaterial().getReflectance(closestIntersection.normal.normalized(), wo.normalized(), wj.normalized());
					lIndir += Utils::multiplyPerCanal(computeColor(ray, objList, lsList), dirReflection);
				}
				lIndir = lIndir / (2 * M_PI * nSample);
			}
			else {
				Ray normalRay = Ray(point, closestIntersection.normal.normalized(), r.depth + 1);
				Vector3D wr = Utils::computeReflectionDirection(wo, closestIntersection.normal.normalized());
				Ray reflectionRay = Ray(point, wr.normalized(), r.depth + 1);
				normalRay.maxT = closestIntersection.normal.length();
				reflectionRay.maxT = wr.length();
				lIndir += (computeColor(normalRay, objList, lsList) + computeColor(reflectionRay, objList, lsList))/(4 * M_PI);
			}
			for (int ls = 0; ls < lsList.size(); ls++) {
				Vector3D wi = lsList[ls].getPosition() - point;
				double d = wi.length();
				wi = wi.normalized();
				if (dot(wi, closestIntersection.normal) >= 0) {
					Ray shadowRay(point, wi);
					shadowRay.maxT = d;
					if (!Utils::hasIntersection(shadowRay, objList)) {
						lDir += Utils::multiplyPerCanal(lsList[ls].getIntensity(point), closestIntersection.shape->getMaterial().getReflectance(closestIntersection.normal, wo, wi));
					}

				}
			}
			
			
			
			
		}


		return lDir + lIndir;
	}

	return bgColor;
}