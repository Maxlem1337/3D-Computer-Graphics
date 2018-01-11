#include "primitives/optimized-mesh-primitive.h"
#include "easylogging++.h"


using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;
using namespace std;

bool sort_vector(std::shared_ptr<Hit> hit1, std::shared_ptr<Hit> hit2) {
	return hit1->t < hit2->t;
}

namespace
{
	class OptimizedMeshImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		vector<Primitive> triangles;
		Box box;
		Primitive left_primitive;
		Primitive right_primitive;
		int depth;

		OptimizedMeshImplementation(vector<Primitive>& primitives, int depth) : triangles(primitives), box(Box::empty()), depth(depth)
		{
			optimise(primitives);
		}



		vector<shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			std::vector<std::shared_ptr<Hit>> hits;
			if (box.is_hit_by(ray)) {
				if (depth == 1) {
					for (auto p : triangles) {
						Hit hit;
						if (p->find_first_positive_hit(ray, &hit)) {
							hits.push_back(make_shared<Hit>(hit));
						}
					}
				}
				for (auto hit : left_primitive->find_all_hits(ray))
				{
					hits.push_back(hit);
				}
				for (auto hit : right_primitive->find_all_hits(ray))
				{
					hits.push_back(hit);
				}
			}

			//std::sort (hits.begin(), hits.end());
			std::sort(hits.begin(), hits.end(), sort_vector);

			return hits;
		}

		Box bounding_box() const override
		{
			return box;
		}

		void optimise(vector<Primitive> primitives) {
			/*
			1. You start with the set of triangles.
			2. Build the smallest bounding box containing all of these triangles. This box is the top of your hierarchy.
			3. Split the box in 2 equal parts. Let's call the two smaller boxes the "left" and "right" box.
			4. Find out in which of these two boxes each triangle fits. There will be triangles that don't fit in either. You can put these in the left box and enlarge it so that it fully envelops these rebel triangles.
			5. Now you have two boxes each containing triangles. Repeat the above steps as often as you want.
			*/

			/* No point in continuing when there are no triangles left. */
			if (primitives.size() == 0) {
				//LOG(INFO) << "[!] Size is 0";
				depth = 1;
				return;
			}

			if (depth == 0) {
				//LOG(INFO) << "Depth is 0, returning!";
				return;
			}



			Box main_box = Box::empty();

			/* Getting smallest possible box. */
			for (auto p : primitives)
			{
				main_box = p->bounding_box().merge(main_box);
			}



			math::Box left_box = math::Box::empty();
			math::Box right_box = math::Box::empty();
			vector<Primitive> left_list;
			vector<Primitive> right_list;

			/* Splitting the box along its largest dimension. */
			auto maxX = main_box.x().upper;
			auto minX = main_box.x().lower;
			auto maxY = main_box.y().upper;
			auto minY = main_box.y().lower;
			auto maxZ = main_box.z().upper;
			auto minZ = main_box.z().lower;


			//LOG(INFO) << depth << ": smallest box is: x(" << minX << "," << maxX << ")" << " y(" << minY << ", " << maxY << ")" << " z(" << minZ << ", " << maxZ << ")";

			if (maxX - minX >= maxY - minY && maxX - minX >= maxZ - minZ) {
				//LOG(INFO) << "Splitting on x";
				//X is largest
				left_box = math::Box(interval(minX, (minX + maxX) / 2), interval(minY, maxY), interval(minZ, maxZ));
				//LOG(INFO) << depth << ": left box is: x(" << minX << "," << (minX + maxX) / 2 << ")" << " y(" << minY << ", " << maxY << ")" << " z(" << minZ << ", " << maxZ << ")";

				right_box = math::Box(interval((minX + maxX) / 2, maxX), interval(minY, maxY), interval(minZ, maxZ));
				//LOG(INFO) << depth << ": right box is: x(" << (minX + maxX) / 2 << "," << maxX << ")" << " y(" << minY << ", " << maxY << ")" << " z(" << minZ << ", " << maxZ << ")";

				/* Put triangles in the correct box */
				for (auto t : primitives)
				{
					if (t->bounding_box().center().x() >= minX && t->bounding_box().center().x() <= (minX + maxX) / 2) {
						left_list.push_back(t);
					}
					else {
						right_list.push_back(t);
					}
				}
			}
			else if (maxY - minY >= maxZ - minZ) {
				//LOG(INFO) << "Splitting on y";
				//Y is largest
				left_box = math::Box(interval(minX, maxX), interval(minY, (minY + maxY) / 2), interval(minZ, maxZ));
				//LOG(INFO) << depth << ": left box is: x(" << minX << "," << maxX << ")" << " y(" << minY << ", " << (minY + maxY) / 2 << ")" << " z(" << minZ << ", " << maxZ << ")";

				right_box = math::Box(interval(minX, maxX), interval((minY + maxY) / 2, maxY), interval(minZ, maxZ));
				//LOG(INFO) << depth << ": right box is: x(" << minX << "," << maxX << ")" << " y(" << (minY + maxY) / 2 << ", " << maxY << ")" << " z(" << minZ << ", " << maxZ << ")";


				/* Put triangles in the correct box */
				for (auto t : primitives)
				{
					if (t->bounding_box().center().y() >= minY && t->bounding_box().center().y() <= (minY + maxY) / 2) {
						left_list.push_back(t);
					}
					else {
						right_list.push_back(t);
					}
				}
			}
			else {
				//LOG(INFO) << "Splitting on z";
				//Z is largest
				left_box = math::Box(interval(minX, maxX), interval(minY, maxY), interval(minZ, (minZ + maxZ) / 2));
				//LOG(INFO) << depth << ": left box is: x(" << minX << "," << maxX << ")" << " y(" << minY << ", " << maxY << ")" << " z(" << minZ << ", " << (minZ + maxZ) / 2 << ")";

				right_box = math::Box(interval(minX, maxX), interval(minY, maxY), interval((minZ + maxZ) / 2, maxZ));
				//LOG(INFO) << depth << ": right box is: x(" << minX << "," << maxX << ")" << " y(" << minY << ", " << maxY << ")" << " z(" << (minZ + maxZ) / 2 << ", " << maxZ << ")";


				/* Put triangles in the correct box */
				for (auto t : primitives)
				{
					if (t->bounding_box().center().z() >= minZ && t->bounding_box().center().z() <= (minZ + maxZ) / 2) {
						left_list.push_back(t);
					}
					else {
						right_list.push_back(t);
					}
				}
			}
			box = main_box;
			//LOG(INFO) << "Size of left: " << left_list.size() << " Size of right: " << right_list.size();
			left_primitive = Primitive(make_shared<OptimizedMeshImplementation>(left_list, depth - 1));
			right_primitive = Primitive(make_shared<OptimizedMeshImplementation>(right_list, depth - 1));
		}
	};
}

Primitive raytracer::primitives::optimized_mesh(vector<Primitive>& triangles)
{
	return Primitive(make_shared<OptimizedMeshImplementation>(triangles, 50));
}
