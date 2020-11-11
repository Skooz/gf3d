#include "simple_logger.h"

#include "gf3d_entity.h"

#include "collision.h"

typedef struct
{
	Entity	*entity_list;	//**<My big fat fuckin' list of really dumb entities*/
	Uint32	entity_count;	//**<upper limit for concurrent active entities*/
}EntityManager;

static EntityManager gf3d_entity = { 0 };

void gf3d_entity_free(Entity *entity); // Declare early; used in entity close

void gf3d_entity_close()
{
	int i;
	if (gf3d_entity.entity_list != NULL)
	{
		for (i = 0; i < gf3d_entity.entity_count; i++)
		{
			gf3d_entity_free(&gf3d_entity.entity_list[i]);
		}
		free(gf3d_entity.entity_list);
	}
	memset(&gf3d_entity, 0, sizeof(EntityManager));
	slog("Entity system closed");
}

void gf3d_entity_init(Uint32 maxEntities)
{
	if (gf3d_entity.entity_list != NULL)
	{
		slog("WARNING: entity system already initialized");
		return;
	}
	gf3d_entity.entity_list = gfc_allocate_array(sizeof(Entity), maxEntities);

	if (!gf3d_entity.entity_list)
	{
		slog("failed to allocate entity list");
		return;
	}
	gf3d_entity.entity_count = maxEntities;
	atexit(gf3d_entity_close);
	slog("Entity system initialized");
}

void gf3d_entity_free(Entity *entity)
{
	if (!entity) return;

	gf3d_model_free(entity->model);
	memset(entity, 0, sizeof(Entity));
}

Entity *gf3d_entity_new()
{
	int i;
	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse)
		{
			gf3d_entity.entity_list[i]._inuse = 1;
			gfc_matrix_identity(gf3d_entity.entity_list[i].modelMatrix);
			return &gf3d_entity.entity_list[i];
		}
	}
	slog("Failed to provide new entity, no unused slots");
	return NULL;
}

void gf3d_entity_think(Entity *self)
{
	if (!self) return;
	if (!self->think) return; // no think function to call

	self->think(self);
	gf3d_entity_collide_check(self);
}

void gf3d_entity_think_all()
{
	int i;

	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_think(&gf3d_entity.entity_list[i]);
	}
}

void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self) return;

	//void gf3d_model_draw(Model *model,Uint32 bufferFrame,VkCommandBuffer commandBuffer,Matrix4 modelMat);
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}

void gf3d_entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i;

	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_draw(&gf3d_entity.entity_list[i], bufferFrame, commandBuffer);
	}
}

void gf3d_entity_collide(Entity *e1, Entity *e2)
{
	if (collide_circle(e1->position, e1->radius, e2->position, e2->radius))
	{
		if (e1->touch)
		{	
			e1->touch(e1, e2);
		}
	}
}

void gf3d_entity_collide_check(Entity *ent)
{
	int i;
	if (!ent) return;
	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		if (&gf3d_entity.entity_list[i] == ent) continue;
		gf3d_entity_collide(ent, &gf3d_entity.entity_list[i]);
	}
}

/*eol@eof*/