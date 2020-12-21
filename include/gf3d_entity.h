#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "stdio.h"
#include "gfc_types.h"
#include "gfc_matrix.h"

#include "gf3d_model.h"


/**
 * @purpose this is the file to access the entity management system
 */
typedef struct Entity_S
{
	Uint8		_inuse; /**<flag to make sure entities are not re-assigned while active*/
	
	// General
	Vector3D	position;
	Vector3D	velocity;
	Vector3D	rotation;
	float		radius;
	Model		*model;
	Matrix4		modelMatrix;
	void(*think)(struct Entity_S *self);
	void(*touch)(struct Entity_S *self, struct Entity_S *other);
	//void		(*die)(struct Entity_S *self);
	//void		(*update)(struct Entity_S *self);
	Uint32		delay;

	// Player 
	int isPlayer;
	float rotHeight;
	float rotCurrent;
	float rotTarget;
	
	// Monsters
	int			monsterType;
	Uint32		nextMove;
	int			moveDir;

	// Stats
	float health;
	float maxHealth;
	float mana;
	float maxMana;
	float stamina;
	float maxStamina;
	float exp;

}Entity;


void gf3d_entity_close();

/**
 * @brief initializes the internal structures for managing the entity system
 * @param maxEntities the maximum number of concurrent entities to be supported
 */
void gf3d_entity_init(Uint32 maxEntities);


/**
* @brief  free an entity so it can be reused by the system
* @param self  the entity to free
* @note  the pointer should be set to zero and no longer used
*/
void gf3d_entity_free(Entity *entity);

/**
 * @brief search for an empty entity slot and returna  pointer to the blank entity
 * @param NULL on error or a pointer to an entity otherwise
 */
Entity *gf3d_entity_new();


void gf3d_entity_think(Entity *self);

/**
* @brief call think function for all active entities
*/
void gf3d_entity_think_all();

/**
* @brief queues up the entity to be drawn for the given draw command
* @param self the entity to draw
* @param bufferFrame the renderFrame to draw with
* @param commandBuffer the command to populate with this draw command
*/
void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer);


/**
 * @brief queues up the entity to be drawn for the given draw command
 * @param bufferFrame the renderFrame to draw with
 * @param commandBuffer the command to populate with this draw command
 */
void gf3d_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);


void gf3d_entity_collide(Entity *e1, Entity *e2);


void gf3d_entity_collide_check(Entity *ent);

#endif