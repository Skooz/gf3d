#include "simple_logger.h"

#include "player.h"

void player_think(Entity *self)
{
	
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

	ent->model = gf3d_model_load(modelName);

	vector3d_copy(ent->position, position);

	ent->think = player_think;

}



/*eol@eof*/