#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "player.h"
#include "collision.h"
#include "fireball.h"

Uint8 *keys;
Vector2D forward;
Vector3D move;
float moveSpeed = 0.02;
Matrix4 shove, rotation, temp;
Uint32 wait;
Uint32 delay = 2000;
// wait = SDL_GetTicks() + delay;

int doAnim;

void player_print_stats(Entity *self)
{
	slog("\nHealth: %f/%f\nMana: %f/%f\nStamina: %f/%f\nEXP: %f", self->health, self->maxHealth, self->mana, self->maxMana, self->stamina, self->maxStamina, self->exp);
}

void player_think(Entity *self)
{
	keys = SDL_GetKeyboardState(NULL);

	// Passive Stat Regen
	if (SDL_GetTicks() % 1000 == 0)
	{
		self->exp += 1;
		
		if (self->mana < self->maxMana)
		{
			self->mana += 2;
		}
		if (self->stamina < self->maxStamina)
		{
			self->stamina += 2;
		}
	}

	// Stat correction
	if (self->health > self->maxHealth)
		self->health = self->maxHealth;
	if (self->health < 0)
		// you're dead anyways lmao
	if (self->mana > self->maxMana)
		self->mana = self->maxMana;
	if (self->mana < 0)
		self->mana = 0;
	if (self->stamina > self->maxStamina)
		self->stamina = self->maxStamina;
	if (self->stamina < 0)
		self->stamina = 0;


	// Additional inputs
	if (wait < SDL_GetTicks()) // Anti input spam
	{
		// leveling
		if (keys[SDL_SCANCODE_F1]) // Display stats
		{
			player_print_stats(self);
			wait = SDL_GetTicks() + delay;
		}
		if (keys[SDL_SCANCODE_F2]) // Upgrade health
		{
			self->maxHealth += 10;
			slog("\nHealth Upgraded (+10)");
			player_print_stats(self);
			wait = SDL_GetTicks() + delay;
		}
		if (keys[SDL_SCANCODE_F3]) // Upgrade mana
		{
			self->maxMana += 10;
			slog("\nMana Upgraded (+10)");
			player_print_stats(self);
			wait = SDL_GetTicks() + delay;
		}
		if (keys[SDL_SCANCODE_F4]) // Upgrade stamina
		{
			self->maxStamina += 10;
			slog("\nMana Upgraded (+10)");
			player_print_stats(self);
			wait = SDL_GetTicks() + delay;
		}

		// heal
		if (keys[SDL_SCANCODE_H] && self->mana > 10 && (self->maxHealth-self->health) >= 10)
		{
			slog("Healed! +10 HP. -10 Mana.");
			self->health += 10;
			self->mana -= 10;
			player_print_stats(self);
			wait = SDL_GetTicks() + delay;
		}

		// BURN, BABY
		if (keys[SDL_SCANCODE_F] && self->mana > 10)
		{
			slog("SWOOSH!!!");
			self->mana -= 10;
			fireball_spawn(self);
			wait = SDL_GetTicks() + delay;
		}

		// SWINGIN' THINGS
		if (keys[SDL_SCANCODE_V] && doAnim == 0)
		{
			doAnim = 1;
			wait = SDL_GetTicks() + delay/4;
		}
	}

	// Sword slice!
	if (doAnim == 1)
	{
		self->animFrame += 0.025;
		if (self->animFrame >= 12)
		{
			self->animFrame = 0;
			doAnim = 0;
		}
	}

	// Movement
	move = vector3d(keys[SDL_SCANCODE_E] - keys[SDL_SCANCODE_Q], keys[SDL_SCANCODE_W] - keys[SDL_SCANCODE_S], 0);
	forward.x = (move.x * SDL_cosf(self->rotCurrent)) - (move.y * SDL_sinf(self->rotCurrent));
	forward.y = (move.x * SDL_sinf(self->rotCurrent)) + (move.y * SDL_cosf(self->rotCurrent));
	self->position.x -= forward.x * moveSpeed;
	self->position.y -= forward.y * moveSpeed;
	//self->rotCurrent = SDL_fmodf(self->rotCurrent, 6.28319);

	gf3d_camera_update(vector3d(self->position.x, self->position.y, self->position.z), vector3d(0, self->rotHeight, self->rotCurrent));

	// Sprint
	if (keys[SDL_SCANCODE_LSHIFT] && self->stamina > 5)
	{
		self->stamina -= 0.015;
		moveSpeed = 0.04;
	}
	if (!keys[SDL_SCANCODE_LSHIFT] || self->stamina < 5)
	{
		moveSpeed = 0.02;
	}

	if (keys[SDL_SCANCODE_W]) // Forward
	{
		
	}
	if (keys[SDL_SCANCODE_S]) // Backward
	{
		
	}
	if (keys[SDL_SCANCODE_A]) // Turn Left
	{
		self->rotCurrent += 0.0015;
		self->rotation.z += 0.0015;
	}
	if (keys[SDL_SCANCODE_D]) // Turn Right
	{
		self->rotCurrent -= 0.0015;
		self->rotation.z -= 0.0015;
	}

	// Gliding (jumping)
	if (keys[SDL_SCANCODE_SPACE] && self->mana > 5)
	{
		self->mana -= 0.015;
		self->position.z += 0.0025;
	}
	if (!keys[SDL_SCANCODE_SPACE] && self->position.z > 0 || self->mana < 5)
	{
		self->position.z -= 0.0025;
	}
	// floor correction
	if (self->position.z < 0)
		self->position.z = 0;
	
	// Rotation for weapon
	gfc_matrix_identity(self->modelMatrix);
	gfc_matrix_rotate(
		rotation,
		self->modelMatrix,
		self->rotation.z,
		vector3d(0, 0, 1));
	gfc_matrix_rotate(
		temp,
		rotation,
		self->rotation.y,
		vector3d(0, 0, 1));
	gfc_matrix_rotate(
		rotation,
		temp,
		self->rotation.x,
		vector3d(0, 0, 1));
	gfc_matrix_make_translation(shove, self->position);
	gfc_matrix_multiply(self->modelMatrix, shove, rotation);

}

void player_touch(Entity *self, Entity *other)
{
	if (!self || !other) return;

	//slog("collision");

	if (doAnim)
	{
		other->health -= 1;
	}
}


/*
Uint8 object_bounds(Vector3D center, float radius, Vector2D *normal)
{
	Uint8 hit = 0;

	if (center.x - radius < level->bounds.x)
	{
		hit = 1;
		if (normal)normal->x = 1;
	}
	if (center.y - radius < level->bounds.y)
	{
		hit = 1;
		if (normal)normal->y = 1;
	}
	if (center.x + radius > level->bounds.x + level->bounds.w)
	{
		hit = 1;
		if (normal)normal->x = -1;
	}
	if (center.y + radius > level->bounds.y + level->bounds.h)
	{
		hit = 1;
		if (normal)normal->y = -1;
	}
	if ((hit) && (normal))
	{
		vector2d_normalize(normal);
	}
	return hit;
}
*/


void player_draw_sword(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer, Uint32 frame)
{
	if (!self) return;

	gf3d_model_draw_animated(self->animodel, bufferFrame, commandBuffer, self->modelMatrix, (Uint32)frame);
}

Entity *player_spawn(Vector3D pos, const char *modelName)
{
	Entity *self;
	
	self = gf3d_entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}

	// Stats
	self->isPlayer = 1;
	self->health	= 100;
	self->maxHealth = self->health;
	self->mana		= 100;
	self->maxMana	= self->mana;
	self->stamina	= 100;
	self->maxStamina = self->stamina;
	self->exp		= 0;
	self->radius	= 2;

	// Set think
	self->think = player_think;
	// Set touch
	self->touch = player_touch;
	// Load model
	//self->model = gf3d_model_load(modelName);
	self->animodel = gf3d_model_load_animated("sword_anim", 1, 13);
	self->animFrame = 1;
	doAnim = 0;
	// Set vectors
	vector3d_copy(self->position, pos);
	vector3d_set(self->rotation, 0, 0, 0);
	// Set init camera pos
	gf3d_vgraphics_move_camera(self->position);
	gfc_matrix_make_translation(self->modelMatrix, self->position);
	self->rotCurrent = -M_PI;
	
	return self;
}

/*eol@eof*/