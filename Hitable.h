#pragma once



#include "vec3.h"
#include "ray2.h"
#include "AABB.h"

class FMaterial;

struct HitRecord
{
	float T;
	Vec3 P;
	Vec3 N;
	float U;
	float V;
	FMaterial *Material;
};


class Hitable
{
public:
	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const = 0;
	//virtual bool CreateAABB(float t0, float t1, AABB &aabb) = 0;
	virtual ~Hitable() {}
};



