#include "vec3.h"

class Matrix4 
{
	float m_mat[16];

public:
	Matrix4();
	Matrix4(float m00, float m01, float m02, float m03,
			float m04, float m05, float m06, float m07,
			float m08, float m09, float m10, float m11,
			float m12, float m13, float m14, float m15);
	Matrix4(const Matrix4&);

	float* getMatrix();
	static Matrix4 Translate(float x, float y, float z);
	static Matrix4 Rotate(vec3 angle);
	Matrix4 Scale(float scale);

	Matrix4& operator=(Matrix4);
	Matrix4 operator*(const Matrix4& m) ;
	vec3 operator*(vec3& v) const;
	float& operator[](int id);

	void Debug();
};
