#include <cstdio>
#include <cstring>
#include <inttypes.h>
#include "pmdData.h"

void pmdData::save(const char* fileName) const {
	char saveBuffer[256];
	FILE* fp = fopen(fileName, "wb");
	if (fp == NULL) {
		throw saveError("file open failed");
	}
#define SAVE(buf, size, nelem, fp, what) \
	if (fwrite((buf), (size), (nelem), (fp)) != (nelem)) { \
		fclose(fp); \
		throw saveError(what " write error"); \
	}

	// write header
	strcpy(saveBuffer, "Pmd");
	SAVE(saveBuffer, 1, 3, fp, "header magic")
	SAVE(&header.version, sizeof(header.version), 1, fp, "header version")
	strncpy(saveBuffer, header.modelName.c_str(), 20);
	for (size_t i = strlen(header.modelName.c_str()) + 1; i < 20; i++) saveBuffer[i] = 0xfd;
	SAVE(saveBuffer, 1, 20, fp, "header model name")
	strncpy(saveBuffer, header.comment.c_str(), 256);
	for (size_t i = strlen(header.comment.c_str()) + 1; i < 256; i++) saveBuffer[i] = 0xfd;
	SAVE(saveBuffer, 1, 256, fp, "header comment")

	// write vertex list
	uint32_t nVertex = vertex.size();
	SAVE(&nVertex, sizeof(nVertex), 1, fp, "number of vertex")
	for (std::vector<vertexData>::const_iterator it = vertex.begin(); it != vertex.end(); it++) {
		float fData[8];
		uint16_t wData[2];
		uint8_t bData[2];
		fData[0] = it->x; fData[1] = it->y; fData[2] = it->z;
		fData[3] = it->nx; fData[4] = it->ny; fData[5] = it->nz;
		fData[6] = it->u; fData[7] = it->v;
		wData[0] = it->boneNum[0]; wData[1] = it->boneNum[1];
		bData[0] = it->boneWeight; bData[1] = it->edgeFlag;
		SAVE(fData, sizeof(*fData), 8, fp, "vertex data")
		SAVE(wData, sizeof(*wData), 2, fp, "vertex data")
		SAVE(bData, sizeof(*bData), 2, fp, "vertex data")
	}

	// write face list
	uint32_t nFace = face.size() * 3;
	SAVE(&nFace, sizeof(nFace), 1, fp, "number of face")
	for (std::vector<faceData>::const_iterator it = face.begin(); it != face.end(); it++) {
		uint16_t wData[3];
		wData[0] = it->vertex1;
		wData[1] = it->vertex2;
		wData[2] = it->vertex3;
		SAVE(wData, sizeof(*wData), 3, fp, "face data")
	}

	// write material list
	uint32_t nMaterial = material.size();
	SAVE(&nMaterial, sizeof(nMaterial), 1, fp, "number of material")
	for (std::vector<materialData>::const_iterator it = material.begin(); it != material.end(); it++) {
		float fData[11];
		uint8_t bData[2];
		uint32_t dData;
		char cData[20];
		fData[0] = it->diffuseR; fData[1] = it->diffuseG; fData[2] = it->diffuseB;
		fData[3] = it->alpha; fData[4] = it->specularity;
		fData[5] = it->specularR; fData[6] = it->specularG; fData[7] = it->specularB;
		fData[8] = it->ambientR; fData[9] = it->ambientG; fData[10] = it->ambientB;
		bData[0] = it->toonIndex; bData[1] = it->edgeFlag;
		dData = it->faceCount * 3;
		size_t fnLen = strlen(it->textureFileName.c_str());
		if (fnLen == 0) {
			memset(cData, 0, 20);
		} else {
			strncpy(cData, it->textureFileName.c_str(), 20);
			for (size_t i = fnLen + 1; i < 20; i++) cData[i] = 0xfd;
		}
		SAVE(fData, sizeof(*fData), 11, fp, "material data")
		SAVE(bData, sizeof(*bData), 2, fp, "material data")
		SAVE(&dData, sizeof(dData), 1, fp, "material data")
		SAVE(cData, sizeof(*cData), 20, fp, "material data")
	}

	// bone (not supported, dummy)
	uint16_t nBone = 2;
	SAVE(&nBone, sizeof(nBone), 1, fp, "number of bone")
	{
		char cData[20] = "model";
		uint16_t wData[2] = {0xffff, 1};
		uint8_t bData = 1;
		uint16_t wData2 = 0;
		float fData[3] = {0.0f, 0.0f, 0.0f};
		for (size_t i = strlen(cData) + 1; i < 20; i++) cData[i] = 0xfd;
		SAVE(cData, 1, 20, fp, "bone data")
		SAVE(wData, sizeof(*wData), 2, fp, "bone data")
		SAVE(&bData, sizeof(bData), 1, fp, "bone data")
		SAVE(&wData2, sizeof(wData2), 1, fp, "bone data")
		SAVE(fData, sizeof(*fData), 3, fp, "bone data")
		strcpy(cData, "arrow");
		wData[0] = 0; wData[1] = 0;
		bData = 7;
		fData[1] = 1.0f;
		SAVE(cData, 1, 20, fp, "bone data")
		SAVE(wData, sizeof(*wData), 2, fp, "bone data")
		SAVE(&bData, sizeof(bData), 1, fp, "bone data")
		SAVE(&wData2, sizeof(wData2), 1, fp, "bone data")
		SAVE(fData, sizeof(*fData), 3, fp, "bone data")
	}

	// IK, skin, skin disp, bone disp name, bone disp (not supported)
	uint8_t dummy[4] = {0, 0, 0, 0};
	SAVE(dummy, sizeof(*dummy), 2, fp, "number of IK")
	SAVE(dummy, sizeof(*dummy), 2, fp, "number of skin")
	SAVE(dummy, sizeof(*dummy), 1, fp, "number of skin disp")
	SAVE(dummy, sizeof(*dummy), 1, fp, "number of bone disp name")
	SAVE(dummy, sizeof(*dummy), 4, fp, "number of bone disp")

	fclose(fp);

#undef SAVE
}
