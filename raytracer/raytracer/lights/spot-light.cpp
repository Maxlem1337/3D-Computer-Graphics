#include "spot-light.h"
#include "easylogging++.h"


using namespace imaging;
using namespace math;
using namespace raytracer;

namespace
{
	class SpotLight : public raytracer::lights::_private_::PointLightImplementation {
	public:
		SpotLight(const math::Point3D& origin, const math::Point3D& destination, const math::Angle& a, const imaging::Color& color) : PointLightImplementation(origin), color(color), destination(destination), angle(a)
		{

		}
	protected:
		raytracer::LightRay cast_lightray_to(const math::Point3D& P) const override
		{
			auto L = m_position;

			Ray ray(L, P);

			//A central shining direction specified by a vector v
			auto v = ray.direction.normalized();

			Vector3D u = (destination - L).normalized();

			//u.dot(v) = cosine of the angle between the two vectors u and v.
			//The spotlight only sends lights to location P if the angle between LP and v is less then angle/2
			if (u.dot(v) >= cos(angle / 2))
			{
				return LightRay(ray, color);
			}
			else
			{
				return LightRay(ray, colors::black());
			}
		}

		imaging::Color color;
		math::Point3D destination;
		math::Angle angle;
	};
}

raytracer::LightSource lights::spot(const Point3D& origin, const Point3D& destination, const Angle& a, const Color& color)
{
	return LightSource(std::make_shared<SpotLight>(origin, destination, a, color));
}
