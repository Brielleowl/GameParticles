//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DO_NOT_MODIFY\OpenGLInterface.h"
#include "ParticleEmitter.h"
#include "Settings.h"

extern PerformanceTimer GlobalTimer;

static const unsigned char squareColors[] =
{
	// ----------------------------
	//  point is actually a quad   
	//  set color on each vertex   
	// ----------------------------
	//    Vert A = Yellow 
	//    Vert B = Yellow
	//    Vert C = Yellow
	//    Vert D = Yellow
	// ----------------------------

	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
};

static const float squareVertices[] =
{
	// --------------------
	//   Vert A = (x,y,z)
	//   Vert B = (x,y,z)
	//   Vert C = (x,y,z)
	//   Vert D = (x,y,z)
	// --------------------

	-0.015f,  -0.015f, 0.0f, // Size of Triangle
	 0.015f,  -0.015f, 0.0f, // Size of Triangle
	-0.015f,   0.015f, 0.0f, // Size of Triangle
	 0.015f,   0.015f, 0.0f, // Size of Triangle
};

ParticleEmitter::ParticleEmitter()
	: start_position(0.0f, 0.0f, 0.0f),
	start_velocity(0.0f, 1.0f, 0.0f),
	//max_life(MAX_LIFE),
	//max_particles(NUM_PARTICLES),
	spawn_frequency(0.0000001),
	last_active_particle(-1),
	//particle_list(NUM_PARTICLES),
	vel_variance(1.0, 4.0, 0.4),
	pos_variance(1.0, 1.0, 1.0),
	scale_variance(2.5),
	headParticle(0)
{
	GlobalTimer.Toc();

	last_spawn = GlobalTimer.TimeInSeconds();
	last_loop = GlobalTimer.TimeInSeconds();
}

//ParticleEmitter::~ParticleEmitter()
//{
//	// do nothing
//}

void ParticleEmitter::SpawnParticle()
{
	// create another particle if there are ones free
	int maxpart = NUM_PARTICLES - 1;
	if (last_active_particle < maxpart)
	{

		// create new particle
		Particle *newParticle = new Particle();

		// initialize the particle
		newParticle->life = 0.0f;
		newParticle->position = start_position;
		newParticle->velocity = start_velocity;
		newParticle->scale = Vect4D(2.0f, 2.0f, 2.0f, 2.0f);

		// apply the variance
		this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);

		// increment count
		last_active_particle++;

		// add to list
		this->addParticleToList(newParticle);

	}
}

void ParticleEmitter::update()
{
	// get current time
	GlobalTimer.Toc();
	float current_time = GlobalTimer.TimeInSeconds();

	// spawn particles


	float time_elapsed = current_time - last_spawn;

	// update
	while (spawn_frequency < time_elapsed)
	{
		// spawn a particle
		this->SpawnParticle();
		// adjust time
		time_elapsed -= spawn_frequency;
		// last time
		last_spawn = current_time;
	}

	// total elapsed
	time_elapsed = current_time - last_loop;

	Particle *p = this->headParticle;
	// walk the particles
	//bufferCount = 0;
	drawBuffer.clear();
	while (p != 0)
	{
		// call every particle and update its position 
		p->Update(time_elapsed);
		drawBuffer.push_back(*p);
		// if it's live is greater that the max_life 
		// and there is some on the list
		// remove node
		if ((p->life > MAX_LIFE) && (last_active_particle > 0))
		{
			// particle to remove
			Particle *s = p;
		
			
			// need to squirrel it away.
			p = p->next;
			//bufferCount++;
			// remove last node
			this->removeParticleFromList(s);

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
			//bufferCount++;
		}
	}

	//move a copy to vector for faster iterations in draw
	//p = this->headParticle;
	

	// clear the buffer
	

	// walk the pointers, add to list
	//while (p != 0)
	//{
	//	// add to buffer
	//	drawBuffer.push_back(*p);

	//	// advance ptr
	//	p = p->next;

	//	// track the current count
	//	//
	//}

	// make sure the counts track (asserts go away in release - relax Christos)
	//assert(bufferCount == (last_active_particle + 1));
	last_loop = current_time;
}

void ParticleEmitter::addParticleToList(Particle *p)
{
	assert(p);
	if (this->headParticle == 0)
	{ // first on list
		this->headParticle = p;
		p->next = 0;
		p->prev = 0;
	}
	else
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = 0;
		headParticle = p;
	}

}

void ParticleEmitter::removeParticleFromList(Particle *p)
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == 0 && p->next == 0)
	{ // only one on the list
		this->headParticle = 0;
	}
	else if (p->prev == 0 && p->next != 0)
	{ // first on the list
		p->next->prev = 0;
		this->headParticle = p->next;
	}
	else if (p->prev != 0 && p->next == 0)
	{ // last on the list 
		p->prev->next = 0;
	}
	else//( p->next != 0  && p->prev !=0 )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	// bye bye
	delete p;
}

void ParticleEmitter::draw()
{
	// get the camera matrix from OpenGL
	// need to get the position     
	Matrix cameraMatrix;

	// get the camera matrix from OpenGL
	glGetFloatv(GL_MODELVIEW_MATRIX, reinterpret_cast<float*>(&cameraMatrix));

	// iterate throught the list of particles
	std::vector<Particle>::iterator it;
	Matrix tmp;

	// get the position from this matrix
	Vect4D camPosVect;
	cameraMatrix.get(Matrix::MATRIX_ROW_3, &camPosVect);

	// OpenGL goo... don't worrry about this
	glVertexPointer(3, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	glEnableClientState(GL_COLOR_ARRAY);

	// camera position
	Matrix transCamera;
	transCamera.setTransMatrix(&camPosVect);

	// particle position
	Matrix transParticle;
	Matrix pivotParticle;
	
	Matrix rotParticle;
	Matrix scaleMatrix;
	Vect4D pivotVect;
	for (it = drawBuffer.begin(); it != drawBuffer.end(); ++it)
	{
		////Temporary matrix
		//Matrix tmp;

		//// get the position from this matrix
		//Vect4D camPosVect;
		//cameraMatrix.get(Matrix::MATRIX_ROW_3, &camPosVect);

		//// OpenGL goo... don't worrry about this
		//glVertexPointer(3, GL_FLOAT, 0, squareVertices);
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
		//glEnableClientState(GL_COLOR_ARRAY);

		//// camera position
		//Matrix transCamera;
		//transCamera.setTransMatrix(&camPosVect);

		//// particle position
		//Matrix transParticle;
		it->position = it->position * 0.35f;
		transParticle.setTransMatrix(&(it->position));

		// rotation matrix
		
		rotParticle.setRotZMatrix(it->rotation);

		// pivot Point
		

		pivotVect.set(1.0f, 0.0f, 50.0f);
		pivotVect *= 20.0 * it->life;

		pivotParticle.setTransMatrix(&pivotVect);

		// scale Matrix
		
		scaleMatrix.setScaleMatrix(&it->scale);

		// total transformation of particle
		tmp = scaleMatrix * transCamera * transParticle *rotParticle * scaleMatrix;

		// set the transformation matrix
		glLoadMatrixf(reinterpret_cast<float*>(&(tmp)));

		// squirrel away matrix for next update
		/*tmp.get(Matrix::MATRIX_ROW_0, &it->curr_Row0);
		tmp.get(Matrix::MATRIX_ROW_1, &it->curr_Row1);
		tmp.get(Matrix::MATRIX_ROW_2, &it->curr_Row2);
		tmp.get(Matrix::MATRIX_ROW_3, &it->curr_Row3);*/

		// draw the trangle strip
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// difference vector     
		/*it->diff_Row0 = it->curr_Row0 - it->prev_Row0;
		it->diff_Row1 = it->curr_Row1 - it->prev_Row1;
		it->diff_Row2 = it->curr_Row2 - it->prev_Row2;
		it->diff_Row3 = it->curr_Row3 - it->prev_Row3;*/

	}

	// delete the buffer
	for (size_t i = 0; i < drawBuffer.size(); i++)
	{
		drawBuffer.pop_back();
	}

	// done with buffer, clear it.
	drawBuffer.clear();
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Add some randomness...

	// --------------------------------------------------------------
	//   Ses it's ugly - I didn't write this so don't bitch at me   |
	//   Sometimes code like this is inside real commerical code    |
	//   ( so now you know how it feels )  |
	//---------------------------------------------------------------

	// x - variance
	float var = static_cast<float>(rand() % 1000) * 0.005f; // Var
	float sign = static_cast<float>(rand() % 2);  // Sign 
	float *t_pos = reinterpret_cast<float*>(&pos);
	float*t_var = &pos_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	// x  - add velocity
	t_pos = &vel[x];
	t_var = &vel_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// correct the sign
	var = 1.5f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	if (sign == 0)
	{
		var *= -1.0;
	}
	sc = sc * var;
}


void ParticleEmitter::GoodBye()
{
	Particle *pTmp = this->headParticle;
	Particle *pDeadMan = nullptr;
	while (pTmp)
	{
		pDeadMan = pTmp;
		pTmp = pTmp->next;
		delete pDeadMan;
	}
}

// End of file
