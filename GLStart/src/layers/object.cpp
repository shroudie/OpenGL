#include "object.h"
#include <sstream>
#include <fstream>
#include <string>

Object::Object(const char* fs) 
{
	string line;
	cout << "instantiate object: " << fs << endl;
	ifstream infile(fs);
	while (getline(infile, line)) {
		float x, y, z;
		if (line[0] == 'v') {
			istringstream iss(line.substr(2));
			cout << line.substr(2, line.size() - 1) << endl;
			if (!(iss >> x >> y >> z)) {
				cout << "error " << endl;
				break;
			} 
			cout << x << ", " << y << ", " << z << endl;
			break;
		}
	}
}

bool Object::is_inside_object(float x, float y)
{
	return false;
}