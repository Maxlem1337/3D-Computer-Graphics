#pragma once

#include "primitives/primitive.h"
#include "primitives/triangle-primitive.h"


namespace raytracer
{
	namespace primitives
	{
		Primitive optimized_mesh(std::vector<Primitive>& triangles);
	}
}