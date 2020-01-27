//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// This is a 4 dimensional Vect4D class
#ifndef Vect4D_H
#define Vect4D_H
#include <xmmintrin.h>
#include <smmintrin.h>  

// includes
#include "Enum.h"

// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;

	Vect4D();
	Vect4D(float tx, float ty, float tz, float tw = 1.0f);
	~Vect4D() = default;
	Vect4D(const Vect4D& tmp);
	Vect4D::Vect4D(const __m128 x);
	void norm(Vect4D &out);
	void set(float tx, float ty, float tz, float tw = 1.0f);

	//Vect4D operator + (Vect4D t);
	const Vect4D Vect4D::operator + (const Vect4D& t)const;
	Vect4D operator - (Vect4D t);
	Vect4D operator * (float scale);

	//void Cross(Vect4D &vin, Vect4D &vout);

	float&operator[](VECT_ENUM e);
	/*float& operator*(float x);*/
	//float SqrtByBisection(float x);
	void operator += (const Vect4D& t);
	void Cross(Vect4D& vin, Vect4D& vout)
	{
		//vout._m = _mm_set_ps(1.0f, (x * vin.y - y * vin.x), (z * vin.x - x * vin.z), (y * vin.z - z * vin.y));
		/*vout.x = (y * vin.z - z * vin.y);
		vout.y = (z * vin.x - x * vin.z);
		vout.z = (x * vin.y - y * vin.x);
		vout.w = 1.0f;*/
		vout._m = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(_m, _m, _MM_SHUFFLE(3, 0, 2, 1)),
			_mm_shuffle_ps(vin._m, vin._m, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(_m, _m, _MM_SHUFFLE(3, 1, 0, 2)),
				_mm_shuffle_ps(vin._m, vin._m, _MM_SHUFFLE(3, 0, 2, 1))));
	};
	const void operator *= (const float& scale)
	{
		this->_m = _mm_mul_ps(this->_m, _mm_set_ps1(scale));
	}


	/*float x;
	float y;
	float z;
	float w;*/

	union
	{
		__m128	_m;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};


#endif  

// End of file

