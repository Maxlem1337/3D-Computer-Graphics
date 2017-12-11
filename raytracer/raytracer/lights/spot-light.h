#pragma once
#include "point-light.h"
#include "light-ray.h"
#include "imaging/color.h"
#include "math/vector.h"


namespace {

	class SpotLight : public raytracer::lights::_private_::PointLightImplementation {
	public:
		SpotLight(const math::Point3D& origin, const math::Point3D& destination, const math::Angle& a, const imaging::Color& color);
	protected:
		raytracer::LightRay cast_lightray_to(const math::Point3D&) const override;
		imaging::Color color;
		math::Point3D destination;
		math::Angle angle;
	};
}

namespace raytracer {
	namespace lights {
		raytracer::LightSource spot(const math::Point3D& origin, const math::Point3D& destination, const math::Angle& a, const imaging::Color& color);
	}
}