#include "primitives/bounding-box-primitive.h"


using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{
	class BoundingBoxAcceleratorImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Primitive prim;
		Box bounding;

		BoundingBoxAcceleratorImplementation(const Primitive prim) : prim(prim), bounding(prim->bounding_box())
		{

		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			if (bounding.is_hit_positively_by(ray)) {
				return prim->find_all_hits(ray);
			}
			return std::vector<std::shared_ptr<Hit>>();
		}

		math::Box bounding_box() const override
		{
			return bounding;
		}
	};
}

Primitive raytracer::primitives::bounding_box_accelerator(const Primitive prim)
{
	return Primitive(std::make_shared<BoundingBoxAcceleratorImplementation>(prim));
}
