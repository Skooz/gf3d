#include "simple_logger.h"

#include "gf3d_particle.h"

#include "collision.h"

#include "gf3d_entity.h"

typedef struct
{
	Particle	*particle_list;	
	Uint32		particle_count;	
}ParticleManager;

static ParticleManager gf3d_particle = { 0 };

// Close the particle system
void gf3d_particle_close()
{
	int i;
	if (gf3d_particle.particle_list != NULL)
	{
		for (i = 0; i < gf3d_particle.particle_count; i++)
		{
			gf3d_particle_free(&gf3d_particle.particle_list[i]);
		}
		free(gf3d_particle.particle_list);
	}
	memset(&gf3d_particle, 0, sizeof(ParticleManager));
	slog("Particle system closed");
}

// Initialize the particle system with param: maxEntities
void gf3d_particle_init(Uint32 maxParticles)
{
	if (gf3d_particle.particle_list != NULL)
	{
		slog("WARNING: particle system already initialized");
		return;
	}
	gf3d_particle.particle_list = gfc_allocate_array(sizeof(Particle), maxParticles);

	if (!gf3d_particle.particle_list)
	{
		slog("failed to allocate particle list");
		return;
	}
	gf3d_particle.particle_count = maxParticles;
	atexit(gf3d_particle_close);
	slog("particle system initialized");
}

// Free an particle with param: particle
void gf3d_particle_free(Particle *particle)
{
	if (!particle) return;

	gf3d_model_free(particle->model);
	memset(particle, 0, sizeof(Particle));
}

// Create a new particle
Particle *gf3d_particle_new()
{
	int i;
	for (i = 0; i < gf3d_particle.particle_count; i++)
	{
		if (!gf3d_particle.particle_list[i]._inuse)
		{
			gf3d_particle.particle_list[i]._inuse = 1;
			gfc_matrix_identity(gf3d_particle.particle_list[i].modelMatrix);
			return &gf3d_particle.particle_list[i];
		}
	}
	slog("Failed to provide new particle, no unused slots");
	return NULL;
}

// Call the particle's think function (if they have one)
void gf3d_particle_think(Particle *self)
{
	if (!self) return;
	if (!self->think) return; // no think function to call

	self->think(self);
	//gf3d_particle_collide_check(self);
}

// Call think functions for each particle
void gf3d_particle_think_all()
{
	int i;

	for (i = 0; i < gf3d_particle.particle_count; i++)
	{
		if (!gf3d_particle.particle_list[i]._inuse) continue;
		gf3d_particle_think(&gf3d_particle.particle_list[i]);
	}
}

// Draw the particle
void gf3d_particle_draw(Particle *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self) return;
	if (!self->model) return;

	//void gf3d_model_draw(Model *model,Uint32 bufferFrame,VkCommandBuffer commandBuffer,Matrix4 modelMat);
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}

// Draw each particle
void gf3d_particle_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i;

	for (i = 0; i < gf3d_particle.particle_count; i++)
	{
		if (!gf3d_particle.particle_list[i]._inuse) continue;
		gf3d_particle_draw(&gf3d_particle.particle_list[i], bufferFrame, commandBuffer);
	}
}

// Determine if entities are colliding, call the particle's touch function if they are
/*
void gf3d_particle_collide(Particle *e1, Particle *e2)
{
	if (collide_circle(e1->position, e1->radius, e2->position, e2->radius))
	{
		if (e1->touch)
		{
			e1->touch(e1, e2);
		}
	}
}
*/

// For each particle, check if a collision is occurring
/*
void gf3d_particle_collide_check(Particle *ent)
{
	int i;
	if (!ent) return;
	for (int i = 0; i < gf3d_particle.particle_count; i++)
	{
		if (!gf3d_particle.particle_list[i]._inuse) continue;
		if (&gf3d_particle.particle_list[i] == ent) continue;
		gf3d_particle_collide(ent, &gf3d_particle.particle_list[i]);
	}
}
*/
/*eol@eof*/