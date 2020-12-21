#include "simple_logger.h"

#include "particle_spawner.h"

#include "sparkle.h"


//RandomFloat()*(topValue-bottomValue) + bottomValue;
//slog("%f", SDL_floorf(gfc_random() * 6));

void sparkle_spawner_think(Particle *self)
{
	float randX = gfc_crandom() * 10;
	float randY = gfc_crandom() * 10;
	Uint32 lt = (Uint32)SDL_floorf(gfc_random()*(2500 - 1000) + 1000);
	int type = (int)SDL_floorf(gfc_random() * 5);

	if (SDL_GetTicks() % 1000 == 0)
	{
		sparkle_spawn(vector3d(self->position.x+randX, self->position.y+randY, self->position.z), lt, type); //+-25 on x/y, +-250 lifetime, 1-5 type
	}

	gfc_matrix_make_translation(self->modelMatrix, self->position);
}

Particle *sparkle_spawner(Vector3D position, const char *modelName)
{
	Particle *self;
	
	self = gf3d_particle_new();
	if (!self)
	{
		slog("failed to spawn a new sparkle particle");
		return NULL;
	}

	// Load model
	self->model = gf3d_model_load(modelName);

	// Set position
	vector3d_copy(self->position, position);
	gfc_matrix_make_translation(self->modelMatrix, self->position);

	// Set think
	self->think = sparkle_spawner_think;
}



/*eol@eof*/