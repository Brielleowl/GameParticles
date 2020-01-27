//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"
//#include "Align.h"
class Align
{
public:
	void* operator new(size_t size)
	{
		void* p = _aligned_malloc(size, 16);
		return p;
	}

	void* operator new[](size_t size)
	{
		void* p = _aligned_malloc(size, 16);
		return p;
	}

		void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator delete[](void* p)
	{
		_aligned_free(p);
	}
};

class Particle : public Align
{
public:
	friend class ParticleEmitter;

	Particle();
	~Particle() = default;
	void Update(const float& time_elapsed);
	void CopyDataOnly(Particle *p);

private:
	

	/*Vect4D	prev_Row0;
	Vect4D	prev_Row1;
	Vect4D  prev_Row2;
	Vect4D  prev_Row3;*/

	Vect4D	position;
	
	Vect4D	velocity;
	Vect4D	scale;
	
	//Vect4D  diff_Row0;
	//Vect4D  diff_Row1;
	//Vect4D  diff_Row2;
	//Vect4D  diff_Row3;
	//

	//Vect4D	curr_Row0;
	//Vect4D	curr_Row1;
	//Vect4D  curr_Row2;
	//Vect4D  curr_Row3;
	Particle* next;
	Particle* prev;
	float	rotation;
	float	life;
	float	rotation_velocity;
};


#endif 

// End of File
