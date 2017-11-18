#pragma once

#include "ray-tracer-v4.h"
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
			class RayTracerV5 : public RayTracerV4
			{
			public:
				virtual TraceResult trace(const Scene&, const math::Ray&) const override;
				virtual TraceResult trace(const Scene&, const math::Ray&, double weight) const;
			protected:
				virtual imaging::Color compute_reflection(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const double&) const;
			};
		}

		RayTracer v5();
	}
}