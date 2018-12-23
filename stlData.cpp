#include <cstdio>
#include <stdint.h>
#include "stlData.h"

stlData stlData::loadBinary(const char* fileName) {
	stlData data;
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL) {
		throw loadError("file open failed");
	}

	char headerString[80];
	if (fread(headerString, 1, 80, fp) != 80) {
		fclose(fp);
		throw loadError("header string load error");
	}
	data.headerString = std::string(headerString, 80);

	uint32_t nTriangles;
	if (fread(&nTriangles, sizeof(nTriangles), 1, fp) != 1) {
		fclose(fp);
		throw loadError("number of triangles load error");
	}
	for (uint32_t i = 0; i < nTriangles; i++) {
		float triangleData[12];
		char unused[2];
		if (fread(triangleData, sizeof(float), 12, fp) != 12) {
			fclose(fp);
			throw loadError("coords load error");
		}
		if (fread(unused, 1, 2, fp) != 2) {
			fclose(fp);
			throw loadError("unused load error");
		}
		data.triangles.push_back(triangle(
			vec(triangleData[0], triangleData[1], triangleData[2]),
			vec(triangleData[3], triangleData[4], triangleData[5]),
			vec(triangleData[6], triangleData[7], triangleData[8]),
			vec(triangleData[9], triangleData[10], triangleData[11])));
	}
	fclose(fp);
	return data;
}
