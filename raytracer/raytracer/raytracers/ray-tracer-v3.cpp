#include "raytracers/ray-tracer-v3.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

Color raytracer::raytracers::_private_::RayTracerV3::compute_specular(const MaterialProperties& material_properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{

	//Light's color
	const auto CL = light_ray.color;

	//Position L of the light
	const auto L = light_ray.ray.origin;

	//The location P of the point on a surface that we need to determine color of.
	const auto P = hit.position;

	//The material's specular color  
	const auto CP = material_properties.specular;

	//The eye's position 
	const auto E = ray.origin;

	//The specular exponent 
	double exponent = material_properties.specular_exponent;
	
	Vector3D direction_of_incomming_light = (P - L).normalized();

	Vector3D direction_of_reflected_light = direction_of_incomming_light.reflect_by(hit.normal);

	Vector3D unit_vector = (E - P).normalized();

	double cos = unit_vector.dot(direction_of_reflected_light);

	//return CL * CP * pow(cos, exponent);

	if (cos > 0) {
		return CL * CP * pow(cos, exponent);
	}
	else {
		return colors::black();
	}
}

Color raytracer::raytracers::_private_::RayTracerV3::process_light_ray(const Scene& scene, const MaterialProperties& material_properties, const Hit& hit, const Ray& ray, const LightRay & lightray) const
{
	Color result = RayTracerV2::process_light_ray(scene, material_properties, hit, ray, lightray);
	result += compute_specular(material_properties, hit, ray, lightray);
	return result;
}
raytracer::RayTracer raytracer::raytracers::v3()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV3>());
}