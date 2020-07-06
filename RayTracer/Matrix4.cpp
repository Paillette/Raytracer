#include "Matrix4.h"
#include <math.h>
#include <iostream>
#include <iomanip>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

Matrix4::Matrix4()
{
	m_mat[0] = 1;  m_mat[1] = 0;  m_mat[2] = 0;  m_mat[3] = 0;
	m_mat[4] = 0;  m_mat[5] = 1;  m_mat[6] = 0;  m_mat[7] = 0;
	m_mat[8] = 0;  m_mat[9] = 0;  m_mat[10] = 1; m_mat[11] = 0;
	m_mat[12] = 0; m_mat[13] = 0; m_mat[14] = 0; m_mat[15] = 1;
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
					float m04, float m05, float m06, float m07,
					float m08, float m09, float m10, float m11,
					float m12, float m13, float m14, float m15)
{
	m_mat[0] = m00;  m_mat[4] = m01;  m_mat[8] = m02;  m_mat[12] = m03;
	m_mat[1] = m04;  m_mat[5] = m05;  m_mat[9] = m06;  m_mat[13] = m07;
	m_mat[2] = m08;  m_mat[6] = m09;  m_mat[10] = m10; m_mat[14] = m11;
	m_mat[3] = m12; m_mat[7] = m13; m_mat[11] = m14; m_mat[15] = m15;
}

Matrix4::Matrix4(const Matrix4& m)
{
	for (int i = 0; i < 16; ++i) 
	{
		m_mat[i] = m.m_mat[i];
	}
}

float* Matrix4::getMatrix()
{
	return m_mat;
}

Matrix4 Matrix4::Translate(float x, float y, float z)
{
	Matrix4 translationMat(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	);

	return translationMat;
}

Matrix4 Matrix4::Rotate(vec3 angle)
{
	Matrix4 rotationMatX(
		1, 0, 0, 0,
		0, cosf(angle.x), -sinf(angle.x), 0,
		0, sinf(angle.x), cosf(angle.x), 0,
		0, 0, 0, 1
	);

	Matrix4 rotationMatY(
		cosf(angle.y), 0, sinf(angle.y), 0,
		0, 1, 0, 0,
		-sinf(angle.y), 0, cosf(angle.y), 0,
		0, 0, 0, 1
	);

	Matrix4 rotationMatZ(
		cosf(angle.z), -sinf(angle.z), 0, 0,
		sinf(angle.z), cosf(angle.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	Matrix4 rotationMat = rotationMatZ * rotationMatY * rotationMatX;

	return rotationMat;
}

Matrix4 Matrix4::Scale(float scale)
{
	Matrix4 scaleMat(
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1
	);

	return scaleMat;
}

Matrix4& Matrix4::operator=(Matrix4 m)
{
	if (this != &m) 
	{
		for (int i = 0; i < 16; ++i)
		{
			this->m_mat[i] = m.m_mat[i];
		}
	}

	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& m )
{
	Matrix4 res;

	for (int i = 0; i < 4; ++i) 
	{
		for (int j = 0; j < 4; ++j)
		{
			res.m_mat[j * 4 + i] = 0;

			for (int k = 0; k < 4; ++k)
			{
				res.m_mat[j * 4 + i] += this->m_mat[k * 4 + i] * m.m_mat[j * 4 + k];
			}
		}
	}

	return res;
}

vec3 Matrix4::operator*(vec3& v) const
{
	vec3 res;

	res.x = m_mat[0] * v.x + m_mat[4] * v.y + m_mat[8] * v.z + m_mat[12];
	res.y = m_mat[1] * v.x + m_mat[5] * v.y + m_mat[9] * v.z + m_mat[13];
	res.z = m_mat[2] * v.x + m_mat[6] * v.y + m_mat[10] * v.z + m_mat[14];

	return res;
}

float& Matrix4::operator[](int id)
{
	return m_mat[id];
}

void Matrix4::Debug()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{

			std::cout << std::setw(10) << this->m_mat[j * 4 + i];
		}

		std::cout << std::endl;
	}
}

