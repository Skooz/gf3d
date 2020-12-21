#include "simple_logger.h"

#include "fountain.h"

int TYPE;

void fountain_think(Entity *self)
{

}

void fountain_touch(Entity *self, Entity *other)
{
	if (!self || !other) return;

	if (other->isPlayer)
	{
		if (TYPE == 0)
		{
			other->exp += 0.01;
		}
		if (TYPE == 1)
		{
			other->mana += 0.01;
		}
		if (TYPE == 3)
		{
			other->health += 0.01;
		}
		if (TYPE == 4)
		{
			other->stamina += 0.01;
		}
	}
}

Entity *fountain_spawn(Vector3D position, int type)
{
	Entity *ent;

	TYPE = type;

	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("failed to spawn a new level entity");
		return NULL;
	}

	// Load model
	//ent->model = gf3d_model_load(modelName);

	// Set position
	vector3d_copy(ent->position, position);
	gfc_matrix_make_translation(ent->modelMatrix, ent->position);

	ent->radius = 15;

	// Set think
	ent->think = fountain_think;
	ent->touch = fountain_touch;
}



/*eol@eof*/