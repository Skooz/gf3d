#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "player.h"
#include "collision.h"


void player_update_camera(Vector3D move, Vector3D rotate)
{
	gf3d_camera_update(move, rotate);
}

void player_think(Entity *self)
{
	// Movement
	Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);

	Vector3D move = vector3d(keys[SDL_SCANCODE_E] - keys[SDL_SCANCODE_Q], keys[SDL_SCANCODE_W] - keys[SDL_SCANCODE_S], 0);
	Vector2D forward;
	forward.x = (move.x * SDL_cosf(self->rotCurrent)) - (move.y * SDL_sinf(self->rotCurrent));
	forward.y = (move.x * SDL_sinf(self->rotCurrent)) + (move.y * SDL_cosf(self->rotCurrent));
	self->position.x -= forward.x * 0.02;
	self->position.y -= forward.y * 0.02;
	//self->rotCurrent = SDL_fmodf(self->rotCurrent, 6.28319);

	player_update_camera(vector3d(self->position.x, self->position.y, self->position.z), vector3d(0, self->rotHeight, self->rotCurrent));

	if (keys[SDL_SCANCODE_W]) // Forward
	{
		//self->velocity.y += 0.01;
	}
	if (keys[SDL_SCANCODE_S]) // Backward
	{
		//self->velocity.y -= 0.01;
	}
	if (keys[SDL_SCANCODE_A]) // Turn Left
	{
		//self->velocity.x -= 0.01;
		self->rotCurrent += 0.0015;
		self->rotation.z += 0.0015;
	}
	if (keys[SDL_SCANCODE_D]) // Turn Right
	{
		//self->velocity.x += 0.01;
		self->rotCurrent -= 0.0015;
		self->rotation.z -= 0.0015;
	}
	if (keys[SDL_SCANCODE_Q]) // Turn Left
	{
		
	}
	if (keys[SDL_SCANCODE_E]) // Turn Right
	{
		
	}
	
	// Rotation for weapon
	Matrix4 shove, rotation, temp;
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
	gfc_matrix_make_translation(shove, self->position);
	gfc_matrix_multiply(self->modelMatrix, shove, rotation);


	

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

	// Stats
	self->health	= 100;
	self->mana		= 100;
	self->stamina	= 100;
	self->exp		= 0;
	self->radius	= 1;

	// Set think
	self->think = player_think;
	// Set touch
	self->touch = player_touch;
	// Load model
	self->model = gf3d_model_load(modelName);
	// Set vectors
	vector3d_copy(self->position, pos);
	vector3d_set(self->rotation, 0, 0, 0);
	// Set init camera pos
	gf3d_vgraphics_move_camera(self->position);
	gfc_matrix_make_translation(self->modelMatrix, self->position);
	self->rotCurrent = -M_PI;
	
	return self;
}

/*eol@eof*/