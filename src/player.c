#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "player.h"
#include "collision.h"


void player_think(Entity *self)
{
	// Movement
	Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) // Forward
	{
		self->velocity.y += 0.01;
		gf3d_vgraphics_move_camera(self->position);
	}
	if (keys[SDL_SCANCODE_S]) // Backward
	{
		self->velocity.y -= 0.01;
		gf3d_vgraphics_move_camera(self->position);
	}
	if (keys[SDL_SCANCODE_A]) // Left
	{
		self->velocity.x -= 0.01;
		gf3d_vgraphics_move_camera(self->position);
		//gf3d_vgraphics_rotate_camera(-0.001);
		//self->rotation.x += 0.01;
	}
	if (keys[SDL_SCANCODE_D]) // Right
	{
		self->velocity.x += 0.01;
		gf3d_vgraphics_move_camera(self->position);
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
		//gf3d_vgraphics_rotate_camera(-0.001, self->position);
	}

	if (vector3d_magnitude(self->velocity) > 0.001)
	{
		vector3d_add(self->position, self->position, self->velocity);
		vector3d_scale(self->velocity, self->velocity, 0.1);
	}
	else
	{
		if (vector3d_magnitude(self->velocity) < 0.001)
		{
			vector3d_clear(self->velocity);
		}

	}
	
	
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
	/**/


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

void player_touch(Entity *self, Entity *other)
{
	if (!self || !other) return;

	slog("collision");

	//vector3d_copy(self->position, vector3d(0, -400, 0));
}


/*
Uint8 object_bounds(Vector3D center, float radius, Vector2D *normal)
{
	Uint8 hit = 0;

	if (center.x - radius < level->bounds.x)
	{
		hit = 1;
		if (normal)normal->x = 1;
	}
	if (center.y - radius < level->bounds.y)
	{
		hit = 1;
		if (normal)normal->y = 1;
	}
	if (center.x + radius > level->bounds.x + level->bounds.w)
	{
		hit = 1;
		if (normal)normal->x = -1;
	}
	if (center.y + radius > level->bounds.y + level->bounds.h)
	{
		hit = 1;
		if (normal)normal->y = -1;
	}
	if ((hit) && (normal))
	{
		vector2d_normalize(normal);
	}
	return hit;
}
*/


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

	self->radius = 3;

	// Set vectors
	vector3d_copy(self->position, pos);
	vector3d_set(self->rotation, 0, 0, 0);

	// Set init camera pos
	gf3d_vgraphics_move_camera(self->position);
	gfc_matrix_make_translation(self->modelMatrix, self->position);

	// Set think
	self->think = player_think;

	// Set touch
	self->touch = player_touch;

	return self;
}



/*eol@eof*/