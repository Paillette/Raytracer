#pragma once
#pragma once
#include <math.h>

class vec2 {

public:

	union {
		float data[2];
		struct {
			float x;
			float y;
		};
	};

	// Constructors

	// Vectors default to 0, 0, 0.
	vec2() {
		x = 0;
		y = 0;
	}

	// Construct with values, 3D
	vec2(float ax, float ay, float az) {
		x = ax;
		y = ay;
	}

	// Construct with values, 2D
	vec2(float ax, float ay) {
		x = ax;
		y = ay;
	}

	// Copy constructor
	vec2(const vec2& o) {
		x = o.x;
		y = o.y;
	}

	// Addition

	vec2 operator+(const vec2& o) {
		return vec2(x + o.x, y + o.y);
	}

	vec2& operator+=(const vec2& o) {
		x += o.x;
		y += o.y;
		return *this;
	}

	// Subtraction

	vec2 operator-() {
		return vec2(-x, -y);
	}

	vec2 operator-(const vec2 o) {
		return vec2(x - o.x, y - o.y);
	}

	vec2& operator-=(const vec2 o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}

	// Multiplication by scalars

	vec2 operator*(const float s) {
		return vec2(x * s, y * s);
	}

	vec2& operator*=(const float s) {
		x *= s;
		y *= s;
		return *this;
	}

	// Division by scalars

	vec2 operator/(const float s) {
		return vec2(x / s, y / s);
	}

	vec2& operator/=(const float s) {
		x /= s;
		y /= s;
		return *this;
	}

	// Dot product

	float operator*(const vec2 o) {
		return (x * o.x) + (y * o.y);
	}

	// An in-place dot product does not exist because
	// the result is not a vector.


	// Length of vector
	float magnitude() {
		return sqrt(magnitude_sqr());
	}

	// Length of vector squared
	float magnitude_sqr() {
		return (x * x) + (y * y);
	}

	// Returns a normalised copy of the vector
	// Will break if it's length is 0
	vec2 normalised() {
		return vec2(*this) / magnitude();
	}

	// Modified the vector so it becomes normalised
	vec2& normalise() {
		(*this) /= magnitude();
		return *this;
	}

	float dot(vec2 b) {
		return (*this).x * b.x + (*this).y * b.y;
	}
};