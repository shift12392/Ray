#pragma once

#include "Sphere.h"
#include "SphereContainer.h"

class FBox : public Hitable
{
public:
	FMaterial *Material;
	Vec3 Min;
	Vec3 Max;
	//std::vector<Hitable*> Hitables;

	SphereContainer *Hitables;

	FBox(Vec3 p0, Vec3 p1, FMaterial *Material)
		:Min(p0), Max(p1), Material(Material), Hitables(new SphereContainer)
	{

		Hitables->Add(new XYRect{ p0.X(),p1.X(),p0.Y(),p1.Y(),p1.Z(),Material });
		Hitables->Add(new FlipNormal{ new XYRect{ p0.X(),p1.X(),p0.Y(),p1.Y(),p0.Z(),Material } });
		Hitables->Add(new YZRect{ p0.Y(),p1.Y(),p0.Z(),p1.Z(),p1.X(),Material });
		Hitables->Add(new FlipNormal{ new YZRect{ p0.Y(),p1.Y(),p0.Z(),p1.Z(),p0.X(),Material } });
		Hitables->Add(new XZRect{ p0.X(),p1.X(),p0.Z(),p1.Z(),p1.Y(),Material });
		Hitables->Add(new FlipNormal{ new XZRect{ p0.X(),p1.X(),p0.Z(),p1.Z(),p0.Y(),Material } });
	}

	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const override
	{
		return Hitables->Hit(R, TMax, TMin, Rec);
	}
};