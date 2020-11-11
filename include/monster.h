#ifndef __MONSTER_H__
#define __MOSNTER_H__

#include "gf3d_entity.h"

Entity *monster_spawn(Vector3D pos, const char *modelname, int type);

#endif