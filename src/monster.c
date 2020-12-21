#include "simple_logger.h"
#include "collision.h"
#include "monster.h"

void monster_think(Entity *self)
{
	if (SDL_GetTicks() > self->nextMove)
	{
		self->nextMove = SDL_GetTicks() + 2000;

		if (self->moveDir == 1)
			self->moveDir = 2;
		else if (self->moveDir == 2)
			self->moveDir = 1;
	}

	switch (self->monsterType)
	{
		case 1:
				if (self->moveDir == 1)
				{
					self->position.x += 0.01;
				}
				else if (self->moveDir == 2)
				{
					self->position.x -= 0.01;
				}
			break;
		case 2:
				if (self->moveDir == 1)
				{
					self->position.y += 0.01;
				}
				else if (self->moveDir == 2)
				{
					self->position.y -= 0.01;
				}
			break;
		case 3:
				if (self->moveDir == 1)
				{
					self->position.x += 0.005;
					self->position.y += 0.005;
				}
				else if (self->moveDir == 2)
				{
					self->position.x -= 0.005;
					self->position.y -= 0.005;
				}
			break;
		case 4:
				if (self->moveDir == 1)
				{
					self->position.x += 0.005;
					self->position.y -= 0.005;
				}
				else if (self->moveDir == 2)
				{
					self->position.x -= 0.005;
					self->position.y += 0.005;
				}
			break;
		case 5:
				if (self->moveDir == 1)
				{
					self->position.z += 0.01;
				}
				else if (self->moveDir == 2)
				{
					self->position.z -= 0.01;
				}
			break;
	}

	if (self->health <= 0)
	{
		gf3d_entity_free(self);
	}

	gfc_matrix_make_translation(self->modelMatrix, self->position);
}

void monster_touch(Entity *self, Entity *other)
{
	if (!self || !other) return;

	if (other->isPlayer)
	{
		other->health -= 0.01;
	}
}

Entity *monster_spawn(Vector3D position, const char *modelName, int type)
{
	Entity *ent;

	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("failed to spawn a new monster entity");
		return NULL;
	}

	ent->health = 1;

	ent->nextMove = 0;
	ent->monsterType = type;
	ent->moveDir = 1;

	ent->radius = 4;

	ent->touch = monster_touch;

	// Load model
	ent->model = gf3d_model_load(modelName);

	// Set position
	vector3d_copy(ent->position, position);
	gfc_matrix_make_translation(ent->modelMatrix, ent->position);

	// Set think
	ent->think = monster_think;
}



/*eol@eof*/