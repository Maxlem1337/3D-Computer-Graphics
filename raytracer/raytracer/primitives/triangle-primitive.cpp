#include "primitives/triangle-primitive.h"
#include "util/misc.h"
#include "math/coordinate-systems.h"
#include "math/quadratic_equation.h"
#include "performance/performance.h"
#include <assert.h>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	class TriangleImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Point3D x;
		Point3D y;
		Point3D z;

		TriangleImplementation(const Point3D x, const Point3D y, const Point3D z) : x(x), y(y), z(z) {

		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			std::vector<std::shared_ptr<Hit>> hits;

			auto n = (y - x).cross(z - x).normalized();
			auto t = (x - ray.origin).dot(n) / ray.direction.dot(n);
			auto h = ray.origin + ray.direction * t;

			if ((y - x).cross(h - x).dot(n) < 0) {
				return hits;
			}

			if ((z - y).cross(h - y).dot(n) < 0) {
				return hits;
			}

			if ((x - z).cross(h - z).dot(n) < 0) {
				return hits;
			}


			auto hit = std::make_shared<Hit>();
			initialize_hit(hit.get(), ray, t);
			hits.push_back(hit);
			return hits;
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			auto n = (y - x).cross(z - x).normalized();
			auto t = (x - ray.origin).dot(n) / ray.direction.dot(n);
			auto h = ray.origin + ray.direction * t;

			//Check primitive.cpp
			if (t < 0 || t >= hit->t) {
				return false;
			}

			if ((y - x).cross(h - x).dot(n) < 0) {
				return false;
			}

			if ((z - y).cross(h - y).dot(n) < 0) {
				return false;
			}

			if ((x - z).cross(h - z).dot(n) < 0) {
				return false;
			}

			initialize_hit(hit, ray, t);
			return true;
		}

		math::Box bounding_box() const override
		{
			//https://stackoverflow.com/questions/15578041/calculating-the-bounding-box-of-a-random-triangle-using-coordinates

			auto minX = std::min(std::min(x.x(), y.x()), z.x());
			auto minY = std::min(std::min(x.y(), y.y()), z.y());
			auto minZ = std::min(std::min(x.z(), y.z()), z.z());

			auto maxX = std::max(std::max(x.x(), y.x()), z.x());
			auto maxY = std::max(std::max(x.y(), y.y()), z.y());
			auto maxZ = std::max(std::max(x.z(), y.z()), z.z());

			return Box(interval(minX, maxX), interval(minY, maxY), interval(minZ, maxZ));
		}

	private:

		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			// Update Hit object
			hit->t = t;
			hit->position = ray.at(t);
			hit->local_position.xyz = hit->position;

			hit->local_position.uv = Point2D(0, 0);

			auto n = (y - x).cross(z - x).normalized();
			hit->normal = n;
		}

	};
}



Primitive raytracer::primitives::triangle(const Point3D x, const Point3D y, const Point3D z)
{
	return Primitive(std::make_shared<TriangleImplementation>(x, y, z));
}