#pragma once

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

static std::string read_file(const char* fp) {
	FILE *f = fopen(fp, "r");
	if (!f) {
		fprintf(stderr, "Cannot open file");
		exit(-1);
	}
	fseek(f, 0, SEEK_END);
	unsigned long size = ftell(f);
	string out(size + 1, 0);
	fseek(f, 0, SEEK_SET);
	fread(&out[0], 1, size, f);;
	fclose(f);
	return out;
}

