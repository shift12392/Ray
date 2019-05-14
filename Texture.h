#pragma once


#include "vec3.h"

class FTexture
{
public:
	virtual Vec3 Value(float U, float V, const Vec3 &P) const = 0;
	virtual ~FTexture() {}
};

class FConstantTexture : public FTexture
{
public:
	FConstantTexture() {}
	FConstantTexture(Vec3 C) :Color(C) {}

	virtual Vec3 Value(float U, float V, const Vec3 &P) const
	{
		return Color;
	}

	Vec3 Color;
};


class FCheckerTexture : public FTexture
{
public:
	FCheckerTexture(FTexture *Tex0, FTexture* Tex1)
		:Odd(Tex0),Even(Tex1)
	{}

	virtual ~FCheckerTexture()
	{
		if (Odd != nullptr)
		{
			delete Odd;
		}
		
		if (Even != nullptr)
		{
			delete Even;
		}
	}

	virtual Vec3 Value(float U, float V, const Vec3 &P) const
	{
		float sines = sin(10.0f * P.X()) * sin(10.0f * P.Y()) * sin(10.0f * P.Z());
		if (sines < 0.0f)
		{
			return Odd->Value(U, V, P);
		}
		else
		{
			return Even->Value(U, V, P);
		}
	}

	FTexture *Odd;
	FTexture *Even;
};

class FImageTexture : public FTexture
{
public:
	FImageTexture() {}
	FImageTexture(unsigned char *pixels,int A,int B)
	:data(pixels),nx(A),ny(B)
	{}

	virtual Vec3 Value(float U, float V, const Vec3 &P) const
	{
		int i = U * nx;
		int j = V * ny;
		if (i < 0)
			i = 0;
		if (j < 0)
			j = 0;
		if (i > nx - 1)
			i = nx - 1;
		if (j > ny - 1)
			j = ny - 1;
		float r = static_cast<int>(data[3 * i + j * nx * 3]) / 255.0f;
		float g = static_cast<int>(data[3 * i + j * nx * 3 + 1]) / 255.0f;
		float b = static_cast<int>(data[3 * i + j * nx * 3] + 2) / 255.0f;
		return Vec3{ r,g,b };
	}

	unsigned char* data=nullptr;
	int nx=0;
	int ny=0;
};
