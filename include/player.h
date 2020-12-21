#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf3d_entity.h"

Entity *player_spawn(Vector3D pos, const char *modelname);

void player_draw_sword(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer, Uint32 frame);

#endif