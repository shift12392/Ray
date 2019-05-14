#pragma once


#include "vec3.h"
#include "ray2.h"



class FCamera
{
public:
	//VFov：垂直方向的视角，以度为单位
	//aspect：宽/高
	FCamera(Vec3 LookFrom,Vec3 LookAt,Vec3 VUp, float VFov, float aspect)
	{
		Vec3 U, V, W;
		float theta = VFov * MY_PI / 180.0f;
		float HalfHeight = tan(theta / 2);
		float HalfWeight = aspect * HalfHeight;
		Origin = LookFrom;
		W = Vec3::UnitVector(LookFrom - LookAt);     
		U = Vec3::UnitVector(Vec3::Cross(VUp, W));
		V = Vec3::Cross(W, U);
		LowerLeftConner = Origin - HalfWeight * U + HalfHeight * V - W;
		HDir = 2 * HalfWeight * U;
		VDir = -2 * HalfHeight * V;
	}

	Ray GetRay(float U, float V)
	{
		return Ray{ Origin,LowerLeftConner - Origin + U * HDir + V * VDir };
	}

	Vec3 Origin{ 0.0f,0.0f,0.0f };
	Vec3 LowerLeftConner{ -2.0f,1.0f,-1.0f };
	Vec3 VDir{ 0.0f,-2.0f,0.0f };   //垂直垂直方向
	Vec3 HDir{ 4.0f,0.0f,0.0f };   //屏幕水平方向  
};
