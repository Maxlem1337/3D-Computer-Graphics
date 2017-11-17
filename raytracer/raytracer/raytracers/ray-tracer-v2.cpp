#include "raytracers/ray-tracer-v2.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV2::trace(const Scene& scene, const Ray& ray) const
{
	Hit hit;

	// Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
	// If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		Color result = colors::black();

		MaterialProperties material_properties = hit.material->at(hit.local_position);

		result += material_properties.ambient;

		result += process_lights(scene, material_properties, hit, ray);
		


		// The hit object contains the group id, just copy it (group ids are important for edge detection)
		unsigned group_id = hit.group_id;

		// The t-value indicates where the ray/scene intersection took place.
		// You can use ray.at(t) to find the xyz-coordinates in space.
		double t = hit.t;

		// Group all this data into a TraceResult object.
		return TraceResult(result, group_id, ray, t);
	}
	else
	{
		// The ray missed all objects in the scene
		// Return a TraceResult object representing "no hit found"
		// which is basically the same as returning black
		return TraceResult::no_hit(ray);
	}
}

Color raytracer::raytracers::_private_::RayTracerV2::compute_diffuse(const MaterialProperties& material_properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{
	//return colors::red();

	//Light's color
	const auto CL = light_ray.color;

	//Position L of the light
	const auto L = light_ray.ray.origin;

	//The location P of the point on a surface that we need to determine color of.
	const auto P = hit.position;

	//The color CS of the surface's material at P
	const auto CS = material_properties.diffuse;

	//The normal vector N on the surface at P
	const auto normal_vector_n = hit.normal;


	//cosine of the angle
	const auto cosine = (L - P).normalized().dot(normal_vector_n);


	if (cosine > 0) {
		return cosine * CL * CS;
	}
	else {
		return colors::black();
	}
}

Color raytracer::raytracers::_private_::RayTracerV2::process_lights(const Scene& scene, const MaterialProperties& material_properties, const Hit& hit, const Ray& ray) const
{
	Color result = colors::black();

	for (int i = 0; i < scene.light_sources.size(); i++)
	{
		result += process_light_source(scene, material_properties, hit, ray, scene.light_sources[i]);
	}

	return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::process_light_source(const Scene& scene, const MaterialProperties& material_properties, const Hit& hit, const Ray& ray, LightSource source) const
{
	Color result = colors::black();

	auto test = source->lightrays_to(hit.position);

	for (auto i = 0; i < test.size(); i++)
	{
		result += process_light_ray(scene, material_properties, hit, ray, test[i]);
	}
	return result;



}

Color raytracer::raytracers::_private_::RayTracerV2::process_light_ray(const Scene& scene, const MaterialProperties& material_properties, const Hit& hit, const Ray& ray, const LightRay & lightray) const
{
	Color result = colors::black();

	result += compute_diffuse(material_properties, hit, ray, lightray);

	return result;
}






raytracer::RayTracer raytracer::raytracers::v2()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV2>());
}