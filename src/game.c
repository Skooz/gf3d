#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "gf3d_entity.h"
#include "gf3d_particle.h"
#include "player.h"
#include "level.h"

int main(int argc,char *argv[])
{
    int done = 0;
	int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
	
	int mousex, mousey;
	float frame = 0;
	Uint32 mouseFrame = 0;
	Sprite *mouse = NULL;
	Sprite *hud = NULL;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

	// START ENTITIES
	gf3d_entity_init(1024);
	gf3d_particle_init(1024);

	Particle *sperkler = sparkle_spawner(vector3d(0,-300,-3), "cone");
	Entity *player = player_spawn(vector3d(0 ,-400, 0), "sword");
	Entity *level = level_spawn(vector3d(0, 0, -5), "level");

	Entity *m1, *m2, *m3, *m4, *m5;
	m1 = monster_spawn(vector3d(-30,-380,0), "cone", 1);
	m2 = monster_spawn(vector3d(-20, -360, 0), "cube", 2);
	m3 = monster_spawn(vector3d(0, -350, 1), "diamond", 3);
	m4 = monster_spawn(vector3d(20, -330, 0), "sphere", 4);
	m5 = monster_spawn(vector3d(40, -320, 0), "tube", 5);

	mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);

	Model *model = NULL;
	Matrix4 modelMat;
	model = gf3d_model_load_animated("sword_anim", 1, 13);
	gfc_matrix_identity(modelMat);
	gfc_matrix_rotate(
		modelMat,
		modelMat,
		M_PI / 2,
		vector3d(1, 0, 0));

	// main game loop
	slog("gf3d main loop begin");
	slog_sync();
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures

        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		SDL_GetMouseState(&mousex, &mousey);
			frame = frame + 0.05;
			if (frame >= 12)frame = 0;
			mouseFrame = (mouseFrame + 1) % 16;

		//update game things here
		// Entities
		gf3d_entity_think_all();
		gf3d_particle_think_all();

        // configure render command for graphics command pool
		bufferFrame = gf3d_vgraphics_render_begin();
        // for each mesh, get a command and configure it from the pool
			gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(), bufferFrame);
			gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(),bufferFrame);

			commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_model_pipeline());
				gf3d_entity_draw_all(bufferFrame, commandBuffer);
				gf3d_particle_draw_all(bufferFrame, commandBuffer);
				//gf3d_model_draw_animated(model, bufferFrame, commandBuffer, modelMat, (Uint32)frame);
				player_draw_sword(player, bufferFrame, commandBuffer, player->animFrame);

			gf3d_command_rendering_end(commandBuffer);
        
		// 2D overlay rendering
			commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_overlay_pipeline());
				//gf3d_sprite_draw(hud, vector2d(0,0), vector2d(2,2), 0, bufferFrame, commandBuffer);
				gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), mouseFrame, bufferFrame, commandBuffer);
			gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);

		if (keys[SDL_SCANCODE_ESCAPE])
		{
			player_save(player, "saves/player.json");
			done = 1; // exit condition
		}
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
	//cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
