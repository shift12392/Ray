#pragma once

#include "Hitable.h"
#include "Material.h"



class Sphere : public Hitable
{
public:
	float Radius = 0;
	Vec3 Center;
	FMaterial *Material;


	Sphere() {}
	Sphere(const Vec3& Center, float Radius , FMaterial *Material)
		:Center(Center), Radius(Radius), Material(Material)
	{

	}

	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const override
	{
		//先进行Hit测试，在计算t。
		Vec3 OC = R.Origin() - Center;
		float a = Vec3::Dot(R.Direction(), R.Direction());
		float b = Vec3::Dot(R.Direction(), OC);
		float c = Vec3::Dot(OC, OC) - Radius * Radius;
		float dota = (b * b - a * c);
		if (dota > 0.0f)
		{
			//计算出t
			float temp = (-b - sqrtf(dota)) / a;
			if (temp < TMax && temp > TMin)
			{
				Rec.T = temp;
				Rec.P = R.PointAtParameter(temp);
				Rec.N = ( Rec.P - Center ) / Radius;
				GetSphereUV(Rec.N, Rec.U, Rec.V);
				Rec.Material = Material;
				return true;
			}
			temp = (-b + sqrtf(dota)) / a;
			if (temp < TMax && temp > TMin)
			{
				Rec.T = temp;
				Rec.P = R.PointAtParameter(temp);
				Rec.N = (Rec.P - Center) / Radius;
				GetSphereUV(Rec.N, Rec.U, Rec.V);
				Rec.Material = Material;
				return true;
			}
		}
		return false;
	}


	static void GetSphereUV(const Vec3& P, float &U, float &V)
	{
		float phi = atan2f(P.Z(), P.X());
		float theta = asinf(P.Y());
		U = (phi + MY_PI) / (2.0f * MY_PI);
		V = (theta + MY_PI / 2.0f) / MY_PI;
	}

};


class XYRect : public Hitable
{
public:
	FMaterial *Material;
	float x0 = -0.5f;
	float x1 = 0.5f;
	float y0 = -0.5f;
	float y1 = 0.5f;
	float k = 0;

	XYRect(float x0, float x1, float y0, float y1,float k,FMaterial* Material)
		:x0(x0), x1(x1), y0(y0), y1(y1),k(k), Material(Material){}

	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const override
	{
		float t = (k - R.Origin().Z()) / R.Direction().Z();
		if (t < TMin || t > TMax)
			return false;
		float x = R.Origin().X() + R.Direction().X() * t;
		float y = R.Origin().Y() + R.Direction().Y() * t;
		if (x < x0 || x > x1 || y <y0 || y > y1)
			return false;

		Rec.Material = Material;
		Rec.N = Vec3(0.0f, 0.0f, 1.0f);
		Rec.P = R.PointAtParameter(t);
		Rec.T = t;
		Rec.U = (x - x0) / (x1 - x0);
		Rec.V = (y - y0) / (y1 - y0);
		return true;
	}

};


class XZRect : public Hitable
{
public:
	FMaterial *Material;
	float x0 = -0.5f;
	float x1 = 0.5f;
	float z0 = -0.5f;
	float z1 = 0.5f;
	float k = 0;

	XZRect(float x0, float x1, float z0, float z1, float k, FMaterial* Material)
		:x0(x0), x1(x1), z0(z0), z1(z1), k(k), Material(Material) {}


	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const override
	{
		float t = (k - R.Origin().Y()) / R.Direction().Y();
		if (t < TMin || t > TMax)
			return false;
		float x = R.Origin().X() + R.Direction().X() * t;
		float z = R.Origin().Z() + R.Direction().Z() * t;
		if (x < x0 || x > x1 || z <z0 || z > z1)
			return false;

		Rec.Material = Material;
		Rec.N = Vec3(0.0f, 1.0f, 0.0f);
		Rec.P = R.PointAtParameter(t);
		Rec.T = t;
		Rec.U = (x - x0) / (x1 - x0);
		Rec.V = (z - z0) / (z1 - z0);
		return true;
	}

};


class YZRect : public Hitable
{
public:
	FMaterial *Material;
	float y0 = -0.5f;
	float y1 = 0.5f;
	float z0 = -0.5f;
	float z1 = 0.5f;
	float k = 0;

	YZRect(float y0, float y1, float z0, float z1, float k, FMaterial* Material)
		:y0(y0), y1(y1), z0(z0), z1(z1), k(k), Material(Material) {}


	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const override
	{
		float t = (k - R.Origin().X()) / R.Direction().X();
		if (t < TMin || t > TMax)
			return false;
		float y = R.Origin().Y() + R.Direction().Y() * t;
		float z = R.Origin().Z() + R.Direction().Z() * t;
		if (y < y0 || y > y1 || z <z0 || z > z1)
			return false;

		Rec.Material = Material;
		Rec.N = Vec3(1.0f, 0.0f, 0.0f);
		Rec.P = R.PointAtParameter(t);
		Rec.T = t;
		Rec.U = (y - y0) / (y1 - y0);
		Rec.V = (z - z0) / (z1 - z0);
		return true;
	}

};


class FlipNormal : public Hitable
{
public:
	Hitable *Ptr = nullptr;

	FlipNormal(Hitable *Ptr) :Ptr(Ptr) {}

	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const override
	{
		if (Ptr->Hit(R, TMax, TMin, Rec))
		{
			Rec.N = -Rec.N;
			return true;
		}
		else
		    return false;
	}
};


