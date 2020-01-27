//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <math.h>
#include "Vect4D.h"

Vect4D::Vect4D():x(0.0f),y(0.0f),z(0.0f),w(1.0f)
{
	/*this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 1.0;*/
}

Vect4D::Vect4D(float tx, float ty, float tz, float tw )/*:x(tx),y(ty),z(tz),w(tw)*/
{
	this->_m = _mm_set_ps(tw, tz, ty, tx);
	/*this->x = tx;
	this->y = ty;
	this->z = tz;
	this->w = tw;*/
}
Vect4D::Vect4D(const Vect4D& tmp):x(tmp.x),y(tmp.y),z(tmp.z),w(tmp.w)
	{

}
Vect4D::Vect4D(const __m128 x)
{
	this->_m = x;
}
//Vect4D::~Vect4D()
//{
//	// nothing to delete
//}
//float Vect4D::SqrtByBisection(float x) {
//	if (x == 0) return 0;
//	float result = x;
//	float xhalf = 0.5f * result;
//	int i = *(int*)&result;
//	i = 0x5f375a86 - (i >> 1);
//	result = *(float*)&i;
//	result = result * (1.5f - xhalf * result * result);
//	result = result * (1.5f - xhalf * result * result);
//	return 1.0f / result;
//}
void Vect4D::norm(Vect4D& out)
{
	//float mag = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	////float tmp = 2 * this->x * this->y + 2 * this->y * this->z + 2 * this->x * this->z;
	////float mag = (this->x + this->y + this->z) - sqrt(tmp);
	///*float xq = this->x * this->x;
	//float yq = this->y * this->y;
	//float zq = this->z * this->z;
	//float a = xq + yq + zq;
	//float mag = SqrtByBisection(a);*/

	//if (0.0f < mag)
	//{
	//	out.x = this->x / mag;
	//	out.y = this->y / mag;
	//	out.z = this->z / mag;
	//	out.w = 1.0;
	//}
	__m128 mag = _mm_sqrt_ps(_mm_dp_ps(_m, _m, 0X77));

	if (0.0f < _mm_cvtss_f32(mag))
	{
		out._m = _mm_div_ps(_m, mag);
		out.w = 1.0f;
	}
}

const Vect4D Vect4D::operator + (const Vect4D& t)const
{
	/*Vect4D out;

	out.x = this->x + t.x;
	out.y = this->y + t.y;
	out.z = this->z + t.z;

	return out;*/
	return Vect4D(_mm_add_ps(this->_m, t._m));
}

Vect4D Vect4D::operator - (Vect4D t)
{
	/*Vect4D out;

	out.x = this->x - t.x;
	out.y = this->y - t.y;
	out.z = this->z - t.z;

	return out;*/
	return Vect4D(this->x - t.x, this->y - t.y, this->z - t.z);
}

Vect4D Vect4D::operator *(float scale)
{
	/*Vect4D tmp;

	tmp.x = this->x * scale;
	tmp.y = this->y * scale;
	tmp.z = this->z * scale;

	return tmp;*/
	return Vect4D(this->x * scale, this->y * scale, this->z * scale);
}

float& Vect4D::operator[](VECT_ENUM e)
{
	VECT_ENUM* p = &e;
	
	while (p != nullptr) {
		if (*p == 0)
			return x;
		else if (*p == 1)
			return y;
		else if (*p == 2)
			return z;
		else if (*p == 3)
			return w;
		else
			return x;
	}
	/*switch (e)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	default:
		assert(0);
		return x;
	}*/

}



void Vect4D::set(float tx, float ty, float tz, float tw)
{
	this->_m = _mm_set_ps(tx, ty, tz, tw);
	/*this->x = tx;
	this->y = ty;
	this->z = tz;
	this->w = tw;*/
}
void Vect4D::operator += (const Vect4D& t)
{
	this->_m = _mm_add_ps(this->_m, t._m);
}

// End of file
