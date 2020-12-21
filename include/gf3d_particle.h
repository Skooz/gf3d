#ifndef __GF3D_PARTICLE_H__
#define __GF3D_PARTICLE_H__

#include "stdio.h"
#include "gfc_types.h"
#include "gfc_matrix.h"

#include "gf3d_model.h"


/**
* @purpose this is the file to access the particle management system
*/
typedef struct Particle_S
{
	Uint8		_inuse; /**<flag to make sure particles are not re-assigned while active*/

	// General
	Vector3D	position;
	Vector3D	rotation;
	Uint32		lifetime;
	float		radius;
	int			particleType;
	Model		*model;
	Matrix4		modelMatrix;
	void(*think)(struct Particle_S *self);
	void(*touch)(struct Particle_S *self, struct Particle_S *other);
	//void		(*die)(struct Entity_S *self);
	//void		(*update)(struct Entity_S *self);

}Particle;


void gf3d_particle_close();


void gf3d_particle_init(Uint32 maxParticles);


void gf3d_particle_free(Particle *part);


Particle *gf3d_particle_new();


void gf3d_particle_think(Particle *self);


void gf3d_particle_think_all();


void gf3d_particle_draw(Particle *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer);


void gf3d_particle_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/*
void gf3d_particle_collide(Particle *p1, Particle *p2);

void gf3d_particle_collide_check(Particle *part);
*/

#endif