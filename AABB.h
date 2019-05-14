#pragma once

#include "vec3.h"
#include "ray2.h"

class AABB {
public:
	AABB() {}
	AABB(const Vec3& a, const Vec3& b) { Min = a; Max = b; }

	Vec3 GetMin() const { return Min; }
	Vec3 GetMax() const { return Max; }

	bool Hit(const Ray& r, float tmin, float tmax) const {
		return true;
	}

	Vec3 Min;
	Vec3 Max;
};