#include "primitives/disk-primitive.h"
#include "math/interval.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

//https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#SECTION00023700000000000000
namespace
{
	class DiskImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Vector3D m_normal = Vector3D(0, 1, 0);
		Point3D center = Point3D(0, 0, 0);
		double radius = 1;


		DiskImplementation()
		{
			// NOP
		}

		math::Box bounding_box() const override
		{
			//TODO!
			//Vector3D e = radius*sqrt((1.0 - m_normal*m_normal));
			//auto a = center - e;
			//auto b = center + e;


			//return Box::infinite();
			//return Box(interval(a.x(), b.x()), interval(a.y(), b.y()), interval(a.z(), b.z()));
			return Box(interval(-radius, radius), interval(-radius, radius), interval(-radius, radius));

		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			std::vector<std::shared_ptr<Hit>> hits;

			auto normal = ray.origin.y() > 0 ? m_normal : -m_normal;

			auto t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);
			auto h = ray.origin + ray.direction * t;

			if ((h-center).dot(h-center) > radius*radius) return hits;

			auto hit = std::make_shared<Hit>();
			initialize_hit(hit.get(), ray, t);
			hits.push_back(hit);
			return hits;
		}


	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
			hit->normal = ray.origin.y() > 0 ? m_normal : -m_normal;
		}
	};
}

Primitive raytracer::primitives::disk()
{
	return Primitive(std::make_shared<DiskImplementation>());
}
