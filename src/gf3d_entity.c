#include "simple_logger.h"

#include "gf3d_entity.h"

#include "collision.h"

typedef struct
{
	Entity	*entity_list;	//**<My big fat fuckin' list of really dumb entities*/
	Uint32	entity_count;	//**<upper limit for concurrent active entities*/
}EntityManager;

static EntityManager gf3d_entity = { 0 };

// Close the entity system
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

// Initialize the entity system with param: maxEntities
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

// Free an entity with param: entity
void gf3d_entity_free(Entity *entity)
{
	if (!entity) return;

	gf3d_model_free(entity->model);
	memset(entity, 0, sizeof(Entity));
}

// Create a new entity
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

// Call the entity's think function (if they have one)
void gf3d_entity_think(Entity *self)
{
	if (!self) return;
	if (!self->think) return; // no think function to call

	self->think(self);
	gf3d_entity_collide_check(self);
}

// Call think functions for each entity
void gf3d_entity_think_all()
{
	int i;

	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_think(&gf3d_entity.entity_list[i]);
	}
}

// Draw the entity
void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self) return;
	if (!self->model) return;

	//void gf3d_model_draw(Model *model,Uint32 bufferFrame,VkCommandBuffer commandBuffer,Matrix4 modelMat);
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}

// Draw each entity
void gf3d_entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i;

	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_draw(&gf3d_entity.entity_list[i], bufferFrame, commandBuffer);
	}
}

// Determine if entities are colliding, call the entity's touch function if they are
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

// For each entity, check if a collision is occurring
void gf3d_entity_collide_check(Entity *ent)
{
	int i;
	if (!ent) return;
	for (int i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		if (&gf3d_entity.entity_list[i] == ent) continue;
		gf3d_entity_collide(ent, &gf3d_entity.entity_list[i]);
	}
}

/*eol@eof*/