#include "simple_logger.h"

#include "player.h"

void player_think(Entity *self)
{
	
	Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) // Forward
	{
		self->position.y -= (float)0.01;
	}
	if (keys[SDL_SCANCODE_S]) // Backward
	{
		self->position.y += (float)0.01;
	}
	if (keys[SDL_SCANCODE_A]) // Left
	{
		self->position.x += (float)0.01;
	}
	if (keys[SDL_SCANCODE_D]) // Right
	{
		self->position.x -= (float)0.01;
	}
	if (keys[SDL_SCANCODE_R]) // Up
	{
		self->position.z += (float)0.01;
	}
	if (keys[SDL_SCANCODE_F]) // Down
	{
		self->position.z -= (float)0.01;
	}
	gfc_matrix_make_translation(self->modelMatrix, self->position);

	/*
	Entity *ent;

	gfc_matrix_rotate(
	self->modelMatrix,
	self->modelMatrix,
	0.002,
	vector3d(1, 0, 0)
	);

	//ent 1 stuff - MAKE MANY AGUMON


	self->delay++;
	if (self->delay == 1000)
	{
	ent = gf3d_entity_new();
	if (ent)
	{
	ent->model = gf3d_model_load("dino");
	ent->think = dino_think;
	gfc_matrix_make_translation(
	ent->modelMatrix,
	vector3d(gfc_crandom() * 5, gfc_crandom() * 5, gfc_crandom() * 5));
	gfc_matrix_rotate(
	ent->modelMatrix,
	ent->modelMatrix,
	gfc_crandom()*0.01,
	vector3d(gfc_crandom() * 5, gfc_crandom() * 5, gfc_crandom() * 5));
	}
	if (self->delay == 2000)
	{
	gf3d_entity_free(self);
	return;
	}
	}
	*/

	/*gfc_matrix_make_translation(
		self->modelMatrix,
		vector3d(gfc_crandom() * 5, gfc_crandom() * 5, gfc_crandom() * 5));
	gfc_matrix_rotate(
		self->modelMatrix,
		self->modelMatrix,
		gfc_crandom()*0.01,
		vector3d(gfc_crandom() * 5, gfc_crandom() * 5, gfc_crandom() * 5));*/

}

Entity *player_spawn(Vector3D position, const char *modelName)
{
	Entity *ent;
	
	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}

	// Load model
	ent->model = gf3d_model_load(modelName);

	// Set position
	vector3d_copy(ent->position, position);
	gfc_matrix_make_translation(ent->modelMatrix, ent->position);

	// Set think
	ent->think = player_think;
}



/*eol@eof*/