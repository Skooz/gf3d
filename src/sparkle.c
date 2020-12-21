#include "simple_logger.h"

#include "sparkle.h"

void sparkle_think(Particle *self)
{
	if (SDL_GetTicks() > self->lifetime)
	{
		gf3d_particle_free(self);
	}

	self->position.z += 0.005;

	gfc_matrix_make_translation(self->modelMatrix, self->position);
}

Particle *sparkle_spawn(Vector3D position, Uint32 lt, int type)
{
	Particle *self;

	self = gf3d_particle_new();
	if (!self)
	{
		slog("failed to spawn a new sparkle particle");
		return NULL;
	}

	//slog("%f, %f, %f", self->position.x, self->position.y, self->position.z);
	//slog("%f", lt);
	self->lifetime = lt + SDL_GetTicks();

	slog("%f", self->lifetime);

	// Load model
	switch (type)
	{
	case 0:
		self->model = gf3d_model_load("cone");
		break;
	case 1:
		self->model = gf3d_model_load("cube");
		break;
	case 2:
		self->model = gf3d_model_load("diamond");
		break;
	case 3:
		self->model = gf3d_model_load("sphere");
		break;
	case 4:
		self->model = gf3d_model_load("tube");
		break;
	case 5:
		self->model = gf3d_model_load("diamond");
		break;
	}

	// Set position
	vector3d_copy(self->position, position);
	gfc_matrix_make_translation(self->modelMatrix, self->position);

	// Set think
	self->think = sparkle_think;
}



/*eol@eof*/