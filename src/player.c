#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "player.h"


void player_think(Entity *self)
{
	// Movement
	Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) // Forward
	{
		self->position.y += 0.01;
	}
	if (keys[SDL_SCANCODE_S]) // Backward
	{
		self->position.y -= 0.01;
	}
	if (keys[SDL_SCANCODE_A]) // Left
	{
		self->position.x -= 0.01;

		//gf3d_vgraphics_rotate_camera(-0.001);
		//self->rotation.x += 0.01;
	}
	if (keys[SDL_SCANCODE_D]) // Right
	{
		self->position.x += 0.01;

		//gf3d_vgraphics_rotate_camera(0.001);
		//self->rotation.x -= 0.01;
	}
	if (keys[SDL_SCANCODE_R]) // Up
	{
		//self->position.z += 0.01;
		self->rotation.z -= 1;
	}
	if (keys[SDL_SCANCODE_F]) // Down
	{
		//self->position.z -= 0.01;
		self->rotation.x -= 1;
	}

	vector3d_add(self->position, self->position, self->velocity);
	
	/*
	Matrix4 move, rotation, temp;
	gfc_matrix_identity(self->modelMatrix);
	gfc_matrix_rotate(
		rotation,
		self->modelMatrix,
		self->rotation.z,
		vector3d(0, 0, 1));
	gfc_matrix_rotate(
		temp,
		rotation,
		self->rotation.y,
		vector3d(0, 0, 1));
	gfc_matrix_rotate(
		rotation,
		temp,
		self->rotation.x,
		vector3d(0, 0, 1));
	gfc_matrix_make_translation(move, self->position);
	gfc_matrix_multiply(self->modelMatrix, move, rotation);
	*/

	gf3d_vgraphics_move_camera(self->position);
	//gf3d_vgraphics_rotate_camera(-0.001);

	Vector3D WeaponPos = self->position;
	WeaponPos.y -= 12;
	WeaponPos.x += 1;
	WeaponPos.z -= 1;
	gfc_matrix_make_translation(self->modelMatrix, WeaponPos);


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

Entity *player_spawn(Vector3D pos, const char *modelName)
{
	Entity *self;
	
	self = gf3d_entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}

	// Load model
	self->model = gf3d_model_load(modelName);

	// Set vectors
	vector3d_copy(self->position, pos);
	vector3d_set(self->rotation, 0, 0, 0);

	gf3d_vgraphics_move_camera(self->position);

	gfc_matrix_make_translation(self->modelMatrix, self->position);

	// Set think
	self->think = player_think;

	return self;
}



/*eol@eof*/