#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include "stlData.h"
#include "pmdData.h"

struct xyz {
	float x, y, z;
	xyz(float x_ = 0, float y_ = 0, float z_ = 0) :
		x(x_), y(y_), z(z_) {}
	xyz(const stlData::vec& v) : x(v.x), y(v.y), z(v.z) {}
	bool operator<(const xyz& o) const {
		if (x != o.x) return x < o.x;
		if (y != o.y) return y < o.y;
		return z < o.z;
	}
};

struct vertex {
	xyz coord;
	xyz norm;
	vertex() {}
	vertex(const xyz& coord_, const xyz& norm_) : coord(coord_), norm(norm_) {}
	bool operator<(const vertex& o) const {
		if (coord < o.coord) return true;
		if (o.coord < coord) return false;
		return norm < o.norm;
	}
};

int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s input_file output_file\n",
			argc > 0 ? argv[0] : "stl2pmd");
		return 1;
	}
	char *sl, *bsl, *fName;
	sl = strrchr(argv[1], '/');
	bsl = strrchr(argv[1], '\\');
	if (sl == NULL && bsl == NULL) fName = argv[1];
	else if (bsl == NULL || bsl < sl) fName = sl + 1;
	else fName = bsl + 1;

	stlData stl = stlData::loadBinary(argv[1]);
	pmdData pmd;
	pmd.header.version = 1.0f;
	pmd.header.modelName = fName;
	pmd.header.comment = stl.headerString;

	std::vector<vertex> vertexList;
	std::map<vertex, int> vertexIndexMap;
	for (std::vector<stlData::triangle>::const_iterator it = stl.triangles.begin();
	it != stl.triangles.end(); it++) {
		int vIdx[3];
		for (int i = 0; i < 3; i++) {
			vertex v(it->vertex[i], it->normal);
			if (vertexIndexMap.find(v) == vertexIndexMap.end()) {
				vertexIndexMap[v] = pmd.vertex.size();
				pmdData::vertexData vd;
				vd.x = v.coord.x; vd.y = v.coord.y; vd.z = v.coord.z;
				vd.nx = v.norm.x; vd.ny = v.norm.y; vd.nz = v.norm.z;
				vd.u = 0.0f; vd.v = 0.0f;
				vd.boneNum[0] = 0; vd.boneNum[1]  = 0; vd.boneWeight = 0;
				vd.edgeFlag = 0;
				pmd.vertex.push_back(vd);
			}
			vIdx[i] = vertexIndexMap[v];
		}
		pmdData::faceData fd;
		fd.vertex1 = vIdx[0];
		fd.vertex2 = vIdx[1];
		fd.vertex3 = vIdx[2];
		pmd.face.push_back(fd);
	}

	pmdData::materialData md;
	md.diffuseR = 0.5f; md.diffuseG = 0.5f; md.diffuseB = 0.5f;
	md.alpha = 1.0f;
	md.specularity = 1.0f;
	md.specularR = 0.5f; md.specularG = 0.5f; md.specularB = 0.5f;
	md.ambientR = 0.1f; md.ambientG = 0.1f; md.ambientB = 0.1f;
	md.toonIndex = 0;
	md.edgeFlag = 0;
	md.faceCount = pmd.face.size();
	pmd.material.push_back(md);

	pmd.save(argv[2]);
	return 0;
}
