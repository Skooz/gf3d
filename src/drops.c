#include "simple_logger.h"

#include "drops.h"

int TYPE;
Uint32 lifetime;

void drop_think(Entity *self)
{
	if (self->position.z > -3)
	{
		self->position.z -= 0.01;
		gfc_matrix_make_translation(self->modelMatrix, self->position);
	}

	gfc_matrix_rotate(
		self->modelMatrix,
		self->modelMatrix,
		0.001,
		vector3d(0, 0, 1));	
}

void drop_touch(Entity *self, Entity *other)
{
	if (!self || !other) return;

	if (other->isPlayer && lifetime < SDL_GetTicks())
	{
		switch (TYPE)
		{
		case 1:
			other->exp += 100;
			slog("Gained 100 EXP!");
			break;
		case 2:
			other->mana += 50;
			slog("Gained 50 MANA!");
			break;
		case 3:
			other->health += 100;
			other->mana += 100;
			other->stamina += 100;
			slog("Gained 100 HEALTH, MANA, & STAMINA!");
			break;
		case 4:
			other->health += 50;
			slog("Gained 50 HEALTH!");
			break;
		case 5:
			slog("Gained 50 STAMINA!");
			other->stamina += 50;
			break;
		}
		gf3d_entity_free(self);
	}
}

Entity *drop_spawn(Vector3D position, int type)
{
	Entity *ent;

	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("failed to spawn a new level entity");
		return NULL;
	}

	lifetime = SDL_GetTicks() + 2500;

	TYPE = type;
	// Load model
	switch(type)
	{
		case 1:
			ent->model = gf3d_model_load("cone");
			break;
		case 2:
			ent->model = gf3d_model_load("cube");
			break;
		case 3:
			ent->model = gf3d_model_load("diamond");
			break;
		case 4:
			ent->model = gf3d_model_load("sphere");
			break;
		case 5:
			ent->model = gf3d_model_load("tube");
			break;
	}
	
	// Set position
	vector3d_copy(ent->position, position);
	gfc_matrix_make_translation(ent->modelMatrix, ent->position);

	ent->radius = 3;
	// Set think
	ent->think = drop_think;
	// Set touch
	ent->touch = drop_touch;
}



/*eol@eof*/