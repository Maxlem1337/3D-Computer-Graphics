#include "raytracers/ray-tracer-v4.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

Color raytracer::raytracers::_private_::RayTracerV4::process_light_ray(const Scene& scene, const MaterialProperties& material_properties, const Hit& hit, const Ray& ray, const LightRay & lightray) const
{
	Color result = RayTracerV3::process_light_ray(scene, material_properties, hit, ray, lightray);

	Hit hit2;

	if (scene.root->find_first_positive_hit(lightray.ray, &hit2)) {
		if (hit2.t >= 0 && hit2.t < 0.99999) {
			return colors::black();
		}
	}


	return result;
}
raytracer::RayTracer raytracer::raytracers::v4()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV4>());
}