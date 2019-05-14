#pragma once

#include "Ray.h"
#include "vec3.h"
#include "Hitable.h"
#include "Texture.h"




class FMaterial
{
public:
	virtual bool Scatter(const Ray& In, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const = 0;
	virtual Vec3 Emitted(float u, float v, const Vec3& p) const { return Vec3{ 0.0f,0.0f,0.0f }; }

	virtual ~FMaterial() {};
public:
	static bool Refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
		Vec3 uv = Vec3::UnitVector(v);
		float dt = Vec3::Dot(uv, n);
		float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
		if (discriminant > 0) {
			refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
			return true;
		}
		else
			return false;
	}
};

extern Vec3 RandomInUnitSphere();

//兰伯特（漫反射）
class FLambertian : public FMaterial
{
public:
	FLambertian() {}
	FLambertian(FTexture *tex) :Albedo(tex) {}
	virtual ~FLambertian() { if (Albedo != nullptr) delete Albedo; }

	FTexture * Albedo;

	virtual bool Scatter(const Ray& In, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		//随机计算一个反射光线
		Vec3 Target = Rec.P + Rec.N + RandomInUnitSphere();
		Scattered = Ray{ Rec.P , Target - Rec.P };

		//
		Attenuation = Albedo->Value(Rec.U,Rec.V,Rec.P);
		return true;
	}
};

extern Vec3 Reflect(const Vec3& In, const Vec3& N);

class FMetal : public FMaterial
{
public:
	Vec3 albedo;
	float fuzz = 0.0f;


	FMetal() {}
	FMetal(Vec3 albedo,float f) :albedo(albedo)
	{
		if (f < 1 && f >= 0.0f)
			fuzz = f;
		else
			fuzz = 1.0f;
	}
	virtual bool Scatter(const Ray& In, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		Vec3 Reflected = Reflect(Vec3::UnitVector(In.Direction()), Rec.N);
		Scattered = Ray{ Rec.P , Reflected + fuzz * RandomInUnitSphere() };
		Attenuation = albedo;
		return (Vec3::Dot(Reflected, Rec.N) > 0.0f);
	}
};

extern float Rand01();

//电解质
class FDielectric : public FMaterial
{
public:
	float RefIdx = 1.0f;
	FDielectric(float RI):RefIdx(RI){}


	/*
	virtual bool Scatter(const Ray& In, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		Vec3 Refracted;
		Vec3 OutNormal;
		Vec3 Reflected = Reflect(In.Direction(), Rec.N);
		Attenuation = Vec3(1.0f, 1.0f, 1.0f);
		float InOverOut;
		if (Vec3::Dot(In.Direction(), Rec.N) > 0)
		{
			OutNormal = -Rec.N;
			InOverOut = RefIdx;
		}
		else
		{
			OutNormal = Rec.N;
			InOverOut = 1.0f / RefIdx;
		}

		if (Refract(In.Direction(), OutNormal, InOverOut, Refracted))
		{
			Scattered = Ray{ Rec.P,Refracted };
		}
		else
		{
			Scattered = Ray{ Rec.P, Reflected };
			return false;   //不返回false，也会出现那种情况
		}

		return true;
	}
	*/

	//计算菲尼尔(Fresnel)反射律
	//Schlick菲尼尔近似相等式
	static float Schlick(float cosine, float ref_idx) {
		float r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0)*pow((1 - cosine), 5);
	}

	virtual bool Scatter(const Ray& In, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		float InOverOut;
		Vec3 Refracted;
		Vec3 OutNormal;
		Vec3 Reflected = Reflect(In.Direction(), Rec.N);
		Attenuation = Vec3(1.0f, 1.0f, 1.0f);
	
		float ReflectProb;          //反射率
		float Cos;
		if (Vec3::Dot(In.Direction(), Rec.N) > 0.0f)
		{//从这个透明物体射出

			OutNormal = -Rec.N;    //法线方向取反
			InOverOut = RefIdx;    // RefIdx/1.0f
			//Cos = RefIdx * Vec3::Dot(In.Direction(), Rec.N) / In.Direction().Length();

			//计算入射光线和法线的夹角的cos值
			Cos = Vec3::Dot(In.Direction(), Rec.N) / In.Direction().Length();
			Cos = sqrt(1 - RefIdx * RefIdx*(1 - Cos * Cos));
		}
		else
		{
			OutNormal = Rec.N;
			InOverOut = 1.0f / RefIdx;    //  空气的折射率/这个透明物体的折射率

			Cos = -Vec3::Dot(In.Direction(), Rec.N) / In.Direction().Length();  //计算入射光线和法线的夹角的cos值。注：法线已经归一化
		}

		if (Refract(In.Direction(), OutNormal, InOverOut, Refracted))
		{
			ReflectProb = Schlick(Cos, RefIdx);    
		}
		else
		{
			ReflectProb = 1.0f;     //全反射
		}


		//根据反射率，在反射或折射之间随机选择并且每次交互仅产生一条散射光线
		if (Rand01() < ReflectProb)
		{
			Scattered = Ray{ Rec.P, Reflected };			
		}
		else
		{
			Scattered = Ray{ Rec.P,Refracted };
		}

		return true;
	}
};

//发光体
class FDiffuseLight : public FMaterial 
{
public:
	FTexture *Emit = nullptr;

	FDiffuseLight(FTexture* tex) :Emit(tex) {}
	~FDiffuseLight() { if (Emit != nullptr)delete Emit; }

	//不散射光线
	virtual bool Scatter(const Ray& In, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const 
	{
		return false;
	}

	//产生能量
	virtual Vec3 Emitted(float u, float v, const Vec3& p) const  override
	{
		return Emit->Value(u, v, p);
	}
};