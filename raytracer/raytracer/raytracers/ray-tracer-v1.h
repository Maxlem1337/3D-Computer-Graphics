#pragma once

#include "raytracers/ray-tracer.h"
#include "materials/material-properties.h"
#include <memory>



namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV1 : public RayTracerImplementation
			{
			public:
				TraceResult trace(const Scene&, const math::Ray&) const override;

			protected:
				imaging::Color compute_ambient(const MaterialProperties&) const;
			};
		}

		RayTracer v1();
	}
}