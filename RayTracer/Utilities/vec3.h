#pragma once

#include <cmath>

struct vec3
{
	union {
		struct {
			float x, y, z;
		};
		struct {
			float r, g, b;
		};
	};

	inline vec3 operator*(const float t) const {
		return{ x*t, y*t, z*t };
	}

	inline vec3 operator+(const vec3& rhs) const {
		return{ x + rhs.x, y + rhs.y, z + rhs.z };
	}

	inline vec3 operator+(const float& rhs) const {
		return{ x + rhs, y + rhs, z + rhs };
	}

	inline vec3 operator-(const vec3& rhs) const {
		return{ x - rhs.x, y - rhs.y, z - rhs.z };
	}

	inline vec3 operator-(const float& rhs) const {
		return{ x - rhs, y - rhs, z - rhs };
	}

	inline vec3 operator*(const vec3& rhs) const {
		return{ x * rhs.x, y * rhs.y, z * rhs.z };
	}

	inline vec3 operator/(const float& rhs) const {
		return{ x / rhs, y / rhs, z / rhs };
	}

	inline vec3& operator +=(const vec3& rhs) {
		this->x = this->x + rhs.x;
		this->y = this->y + rhs.y;
		this->z = this->z + rhs.z;
		return *this;
	}

	inline float dot(const vec3& rhs) const {
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	inline vec3 operator^(const vec3& rhs)
	{
		vec3 v;
		v.x = y * rhs.z - z * rhs.y;
		v.y = z * rhs.x - x * rhs.z;
		v.z = x * rhs.y - y * rhs.x;
		return v;
	}

	inline vec3& normalize() {
		float inv_length = 1.f / sqrtf(x*x + y*y + z*z);
		x *= inv_length;
		y *= inv_length;
		z *= inv_length;
		return *this;
	}

	inline float distance(const vec3& rhs) const
	{
		vec3 v = rhs - *this;
		return sqrt(std::pow(v.x, 2.0f) + std::pow(v.y, 2.0f) + std::pow(v.z, 2.0f));
	}
};

// equivalent a typedef vec3 color;
using color = vec3;
