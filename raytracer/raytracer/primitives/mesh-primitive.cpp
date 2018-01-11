#include "primitives/mesh-primitive.h"
#include "primitives/bounding-box-primitive.h"
#include "easylogging++.h"


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

		Box bounding_box() const override
		{
			LOG(INFO) << "[MESH] Starting bb mesh";
			//get bounding box of first triangle.
			Box smallbox = triangles.at(0)->bounding_box();

			//group all boxes until you end up with on single box
			for each (Primitive p in triangles)
			{
				smallbox = smallbox.merge(p->bounding_box());
			}

			LOG(INFO) << "[MESH] Returning bb mesh";
			return smallbox;
		}
	};
}

Primitive raytracer::primitives::mesh(const vector<Primitive>& triangles)
{
	return Primitive(make_shared<MeshImplementation>(triangles));
}
