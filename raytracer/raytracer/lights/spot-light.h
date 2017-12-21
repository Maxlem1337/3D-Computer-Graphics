#pragma once
#include "point-light.h"
#include "light-ray.h"
#include "imaging/color.h"
#include "math/vector.h"

namespace raytracer {
	namespace lights {
		raytracer::LightSource spot(const math::Point3D& origin, const math::Point3D& destination, const math::Angle& a, const imaging::Color& color);
	}
}