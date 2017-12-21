#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>

#include "mesh-reader.h"
#include "easylogging++.h"
#include "primitives\primitives.h"

using namespace std;
using namespace math;


//Only works for .ply mesh files.
raytracer::Primitive raytracer::mesh::MeshReader::read_mesh(std::string path)
{
	//Vertex is een punt waar 2 of meerdere lijnen samenkomen.
	int vertex = 0;
	//face is een oppervlakte van een object.
	int faces = 0;
	vector<Primitive> faces_list;
	vector<Point3D> vertex_list;


	LOG(INFO) << "Starting";
	//path = "E:\\Programs\\Dropbox\\Dropbox\\UCLL\\3d\\bunny\\reconstruction\\bun_zipper_res4.ply";
	string line;
	ifstream myfile(path);
	if (myfile.is_open())
	{
		bool reading_configuration = true;
		//read configuration
		while (reading_configuration) {

			//Line 1 should be "ply"
			getline(myfile, line);
			assert(line == "ply");
			LOG(INFO) << "File is PLY";

			//Line 2 is format, only tested with ascii 1.0 No idea what other formats look like.
			getline(myfile, line);
			assert(line == "format ascii 1.0");
			LOG(INFO) << "File is ascii 1.0";

			//Keep reading until element vertex, element face or end_header.
			while (getline(myfile, line))
			{
				if (line == "end_header") {
					reading_configuration = false;
					break;
				}
				else if (starts_with("element vertex", line)) {
					vector<string> split_string = split(line, ' ');
					//I should probably add a try catch here.
					vertex = atoi(split_string.at(2).c_str());
					LOG(INFO) << "Number of vertex: " << vertex;
				}
				else if (starts_with("element face", line)) {
					vector<string> split_string = split(line, ' ');
					//I should probably add a try catch here.
					faces = atoi(split_string.at(2).c_str());
					LOG(INFO) << "Number of faces: " << faces;
				}
			}
		}
		for (int i = 0; i < vertex; i++) {
			getline(myfile, line);
			auto doubles = split(line, ' ');
			double x = atof(doubles.at(0).c_str());
			double y = atof(doubles.at(1).c_str());
			double z = atof(doubles.at(2).c_str());
			vertex_list.push_back(Point3D(x, y, z));
		}

		for (int i = 0; i < faces; i++) {
			getline(myfile, line);
			auto doubles = split(line, ' ');

			int vertex1 = atoi(doubles.at(1).c_str());
			int vertex2 = atoi(doubles.at(2).c_str());
			int vertex3 = atoi(doubles.at(3).c_str());

			//LOG(INFO) << vertex1 << " " << vertex2 << " " << vertex3;

			auto x = vertex_list.at(vertex1);
			auto y = vertex_list.at(vertex2);
			auto z = vertex_list.at(vertex3);
			faces_list.push_back(primitives::triangle(x, y, z));
		}

		myfile.close();
	}
	else LOG(INFO) << "Unable to open file";

	Primitive mesh = primitives::mesh(faces_list);
	LOG(INFO) << "Done!";
	return mesh;
}

bool raytracer::mesh::MeshReader::starts_with(std::string prefix, std::string argument) {
	return argument.substr(0, prefix.size()) == prefix;
}

//https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
template<typename Out>
void raytracer::mesh::MeshReader::split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> raytracer::mesh::MeshReader::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}