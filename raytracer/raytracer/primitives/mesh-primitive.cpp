#include "primitives/mesh-primitive.h"
#include "primitives/bounding-box-primitive.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;
using namespace std;

namespace
{
	class MeshImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		vector<Primitive> triangles;

		MeshImplementation(const vector<Primitive>& triangles) : triangles(triangles)
		{

		}

		vector<shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			vector<shared_ptr<Hit>> hits;

			for (auto p : triangles) {
				Hit hit;
				if (p->find_first_positive_hit(ray, &hit)) {
					hits.push_back(make_shared<Hit>(hit));
				}
			}

			return hits;
		}

		math::Box bounding_box() const override
		{
			//Primitive p = Primitive(make_shared<MeshImplementation>(triangles));
			//return bounding_box_accelerator(p)->bounding_box();

			//Top box
			//Box top_box = Box();




			return Box(math::Interval<double>(-1, 1), math::Interval<double>(-1, 1), math::Interval<double>(-1, 1));
		}
	};
}

Primitive raytracer::primitives::mesh(const vector<Primitive>& triangles)
{
	return Primitive(make_shared<MeshImplementation>(triangles));
}
