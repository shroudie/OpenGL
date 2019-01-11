#include "customObject.h"
#include <sstream>
#include <fstream>
#include <string>

customObject::customObject(const char* fs)
{
	this->two_dimentional = false;
	this->set_layer_name("custom object");
	string line;
	ifstream infile(fs);
	while (getline(infile, line)) {
		if (line[0] == 'v') {
			GLfloat x, y, z; char a;
			sscanf(line.c_str(), "%c %f %f %f", &a, &x, &y, &z);
			buffers.vertices.push_back(x);
			buffers.vertices.push_back(y);
			buffers.vertices.push_back(z);
		}
		else if (line[0] == 'f') {
			GLuint x, y, z; char a;
			sscanf(line.c_str(), "%c %u %u %u", &a, &x, &y, &z);
			buffers.indices.push_back(x-1);
			buffers.indices.push_back(y-1);
			buffers.indices.push_back(z-1);
		}
	}
	this->generate_normals();
	this->load_buffers();
}

bool customObject::is_inside_object(float x, float y)
{
	return false;
}

void customObject::generate_normals()
{
	buffers.normals = vector<GLfloat>(buffers.vertices.size());
	for (unsigned int i = 0; i < buffers.indices.size()/3; ++i) {
		unsigned int v1 = buffers.indices[3 * i], v2 = buffers.indices[3 * i + 1], v3 = buffers.indices[3 * i + 2];
		vec3 v1vec = vec3(buffers.vertices[3 * v1], buffers.vertices[3 * v1 + 1], buffers.vertices[3 * v1 + 2]),
			v2vec = vec3(buffers.vertices[3 * v2], buffers.vertices[3 * v2 + 1], buffers.vertices[3 * v2 + 2]),
			v3vec = vec3(buffers.vertices[3 * v3], buffers.vertices[3 * v3 + 1], buffers.vertices[3 * v3 + 2]);
		vec3 v21 = v2vec - v1vec, v31 = v3vec - v1vec;
		vec3 n = vec3::cross_product(v21, v31);

		buffers.normals[3 * v1] += n[0];
		buffers.normals[3 * v1 + 1] += n[1];
		buffers.normals[3 * v1 + 2] += n[2];
		buffers.normals[3 * v2] += n[0];
		buffers.normals[3 * v2 + 1] += n[1];
		buffers.normals[3 * v2 + 2] += n[2];
		buffers.normals[3 * v3] += n[0];
		buffers.normals[3 * v3 + 1] += n[1];
		buffers.normals[3 * v3 + 2] += n[2];
	}
	for (unsigned int i = 0; i < buffers.normals.size(); i += 3) {
		vec3 n = vec3::normalize(vec3(buffers.normals[i], buffers.normals[i + 1], buffers.normals[i + 2]));
		buffers.normals[i] = n[0];
		buffers.normals[i+1] = n[1];
		buffers.normals[i+2] = n[2];
	}
}