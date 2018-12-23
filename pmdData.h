#ifndef PMDDATA_H_GUARD_B4BC587B_0D47_432B_B835_07EBCD3C0E9C
#define PMDDATA_H_GUARD_B4BC587B_0D47_432B_B835_07EBCD3C0E9C

#include <string>
#include <vector>
#include <stdint.h>

struct pmdData {
	struct headerData {
		float version;
		std::string modelName, comment;
	};
	struct vertexData {
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		int boneNum[2], boneWeight, edgeFlag;
	};
	struct faceData {
		int vertex1, vertex2, vertex3;
	};
	struct materialData {
		float diffuseR, diffuseG, diffuseB;
		float alpha;
		float specularity;
		float specularR, specularG, specularB;
		float ambientR, ambientG, ambientB;
		int toonIndex, edgeFlag, faceCount;
		std::string textureFileName;
	};
	class saveError {
		std::string message;
	public:
		saveError(const std::string& message_) : message(message_) {}
		std::string getMessage() const {
			return message;
		}
	};
	headerData header;
	std::vector<vertexData> vertex;
	std::vector<faceData> face;
	std::vector<materialData> material;

	void save(const char* fileName) const;
};

#endif
