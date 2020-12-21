#include "simple_logger.h"

#include "level.h"

void level_think(Entity *self)
{
	
}

Entity *level_spawn(Vector3D position, const char *modelName)
{
	Entity *ent;
	
	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("failed to spawn a new level entity");
		return NULL;
	}

	// Load model
	ent->model = gf3d_model_load(modelName);

	// Set position
	vector3d_copy(ent->position, position);
	gfc_matrix_make_translation(ent->modelMatrix, ent->position);

	// Set think
	ent->think = level_think;
}



/*eol@eof*/