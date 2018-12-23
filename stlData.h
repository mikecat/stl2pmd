#ifndef STLDATA_H_GUARD_A475E6CC_E28C_426C_93AB_642AE7B3BA0F
#define STLDATA_H_GUARD_A475E6CC_E28C_426C_93AB_642AE7B3BA0F

#include <string>
#include <vector>

struct stlData {
	struct vec {
		float x, y, z;
		vec(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}
	};
	struct triangle {
		vec normal;
		vec vertex[3];
		triangle() {}
		triangle(const vec& normal_, const vec& v0, const vec& v1, const vec& v2) :
		normal(normal_) {
			vertex[0] = v0;
			vertex[1] = v1;
			vertex[2] = v2;
		}
	};
	class loadError {
		std::string message;
	public:
		loadError(const std::string& message_) : message(message_) {}
		std::string getMessage() const {
			return message;
		}
	};
	std::string headerString;
	std::vector<triangle> triangles;

	static stlData loadBinary(const char* fileName);
};

#endif
