#pragma once

#include "ray-tracer-v3.h"
#include "raytracers/ray-tracer.h"
#include "materials/material-properties.h"
#include "lights/light-ray.h"
#include "lights/light-source.h"
#include <memory>

namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV4 : public RayTracerV3
			{
			protected:
				virtual imaging::Color process_light_ray(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const override;
			};
		}

		RayTracer v4();
	}
}