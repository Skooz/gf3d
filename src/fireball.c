#include "simple_logger.h"

#include "fireball.h"

Entity *caster;
Vector2D forward;
Vector3D move;
float dieTime;

void fireball_think(Entity *self)
{
	// Fireball movement
	move = vector3d(0, 1, 0);
	forward.x = (move.x * SDL_cosf(caster->rotCurrent)) - (move.y * SDL_sinf(caster->rotCurrent));
	forward.y = (move.x * SDL_sinf(caster->rotCurrent)) + (move.y * SDL_cosf(caster->rotCurrent));
	self->position.x -= forward.x * 0.05;
	self->position.y -= forward.y * 0.05;

	gfc_matrix_make_translation(self->modelMatrix, self->position);

	if (SDL_GetTicks() > dieTime)
		gf3d_entity_free(self);
}

void fireball_touch(Entity *self, Entity *other)
{
	if (!self || !other) return;

	if (!other->isPlayer)
	{
		other->health -= 1;
		gf3d_entity_free(self);
	}
}

Entity *fireball_spawn(Entity *cstr)
{
	Entity *ent;
	caster = cstr;

	dieTime = SDL_GetTicks() + 5000;

	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("failed to spawn a new fireball entity");
		return NULL;
	}

	// Load model
	ent->model = gf3d_model_load("sphere");

	// Set position
	vector3d_copy(ent->position, caster->position);
	gfc_matrix_make_translation(ent->modelMatrix, ent->position);

	// Set think
	ent->think = fireball_think;
	// Set touch
	ent->touch = fireball_touch;
}

/*eol@eof*/