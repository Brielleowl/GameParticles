//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"

#include <vector>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter() = default;

	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(Particle *p);
	void removeParticleFromList(Particle *p);

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);
	void GoodBye();

private:

	Vect4D	start_position;
	Vect4D	start_velocity;

	//float	max_life;
	//int		max_particles;
	int		last_active_particle;
	float	spawn_frequency;
	float	last_spawn;
	float	last_loop;


	// added for speed efficiency
	//int bufferCount;
	Particle *headParticle;
	Vect4D	vel_variance;
	Vect4D	pos_variance;
	float	scale_variance;
	std::vector<Particle> drawBuffer;
	//std::list<Particle> particle_list;











};

#endif

// End of File
