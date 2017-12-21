#include "intersection-primitive.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;
using namespace std;

namespace
{
	class IntersectionImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		IntersectionImplementation(Primitive p1, Primitive p2) : p1(p1), p2(p2) {

		}

		//vector<shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
		//	vector<shared_ptr<Hit>> hits;

		//	Hit hit;
		//	Hit hit2;
		//	if (p1->find_first_positive_hit(ray, &hit) && p2->find_first_positive_hit(ray, &hit2)) {
		//		hits.push_back(make_shared<Hit>(hit));
		//		hits.push_back(make_shared<Hit>(hit2));
		//	}

		//	return hits;
		//}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> result;

			bool inside1 = false;
			bool inside2 = false;
			bool was_inside_intersection = false;

			// Collect children's hits
			auto hits1 = p1->find_all_hits(ray);
			auto hits2 = p2->find_all_hits(ray);

			// Append hits at infinity
			hits1.push_back(std::make_shared<Hit>());
			hits2.push_back(std::make_shared<Hit>());

			// Start iterating over hit lists
			auto i1 = hits1.begin();
			auto i2 = hits2.begin();

			while (i1 + 1 != hits1.end() || i2 + 1 != hits2.end())
			{
				// Get hits
				auto hit1 = *i1;
				auto hit2 = *i2;
				std::shared_ptr<Hit> active_hit = nullptr;

				// Get t-values of hits
				double t1 = hit1->t;
				double t2 = hit2->t;

				// Find closest hit
				if (t1 < t2)
				{
					// If we were inside child1, we're now outside, and vice versa
					inside1 = !inside1;

					// Save hit for later
					active_hit = hit1;

					// Move to next hit
					++i1;
				}
				else if (t1 > t2)
				{
					// If we were inside child1, we're now outside, and vice versa
					inside2 = !inside2;

					// Save hit for later
					active_hit = hit2;

					// Move to next hit
					++i2;
				}
				else
				{
					inside1 = !inside1;
					inside2 = !inside2;

					active_hit = hit1;

					++i1;
					++i2;
				}

				// We're inside the union if we're inside one of the children
				bool inside_intersection = inside1 && inside2;

				// Check if there was a change
				if (was_inside_intersection != inside_intersection)
				{
					// The active hit is important; add it to the result list
					result.push_back(active_hit);

					// Overwrite value
					was_inside_intersection = inside_intersection;
				}
			}

			return result;
		}


		math::Box bounding_box() const override
		{
			return Box(math::Interval<double>(-1, 1), math::Interval<double>(-1, 1), math::Interval<double>(-1, 1));
		}

	private:
		Primitive p1;
		Primitive p2;
	};

}

Primitive raytracer::primitives::intersection(Primitive p1, Primitive p2)
{
	return Primitive(std::make_shared<IntersectionImplementation>(p1, p2));
}
