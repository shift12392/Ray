#pragma once


#include "Hitable.h"
#include <vector>

class SphereContainer :public Hitable
{
public:

	std::vector<Hitable*> Hitables;

	SphereContainer() {}

	void Add(Hitable *H) { Hitables.push_back(H); }

	virtual bool Hit(const Ray& R, float TMax, float TMin, HitRecord &Rec) const
	{
		float TempT = TMax;
		HitRecord TempHitRecord;
		bool bFindAnything = false;
		for (size_t i = 0; i < Hitables.size(); ++i)
		{
			Hitable *H = Hitables[i];
			if (H->Hit(R, TempT, TMin, TempHitRecord))
			{
				bFindAnything = true;
				TempT = TempHitRecord.T;
				Rec = TempHitRecord;
			}
		}

		return bFindAnything;
	}
};