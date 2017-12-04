#pragma once
#include "primitives/primitive.h"

namespace raytracer
{
	namespace mesh
	{
		class MeshReader
		{
		public:
			Primitive read_mesh(std::string path);
		private:
			bool starts_with(std::string prefix, std::string argument);
			std::vector<std::string> split(const std::string &s, char delim);
			template<typename Out>
			void split(const std::string &s, char delim, Out result);
		};

	}
}