#include "raytracers/ray-tracer-v5.h"

using namespace imaging;
using namespace math;
using namespace raytracer;



raytracer::RayTracer raytracer::raytracers::v5()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV5>());
}

TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray) const
{
	return trace(scene, ray, 1.0);
}

TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray, double weight) const
{
	Hit hit;

	// Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
	// If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
	if (scene.root->find_first_positive_hit(ray, &hit) && weight > 0.01)
	{
		Color result = colors::black();

		MaterialProperties material_properties = hit.material->at(hit.local_position);


		//Compute ambient
		result += material_properties.ambient;

		//Compute diffuse + specular
		result += process_lights(scene, material_properties, hit, ray);

		//Compute reflection.
		result += compute_reflection(scene, material_properties, hit, ray, weight);

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

imaging::Color raytracer::raytracers::_private_::RayTracerV5::compute_reflection(const Scene& scene, const MaterialProperties& material_properties, const Hit& hit, const math::Ray& ray, const double& weight) const
{
	auto reflectivity = material_properties.reflectivity;
	
	if (reflectivity > 0) {
		auto reflected_direction = ray.direction.reflect_by(hit.normal);
		auto reflected_ray = math::Ray(hit.position + reflected_direction * 0.00000001, reflected_direction);

		return reflectivity * trace(scene, reflected_ray, weight * reflectivity).color;
	}
	else {
		return colors::black();
	}
}


