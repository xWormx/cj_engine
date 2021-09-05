// TODO(calle): fixa så att DrawText() kan printa olika storlekar på fonts..... kolla i cj_renderer.h
// 				- Draw() i UpdateRenderer() måste veta vilken texture_batch / tex_i some ska användas 

bool running = true;

#include "cj_engine.h"

GLenum err = 0;

int main()
{
    
	CJ_PLATFORM platform 	= CreatePlatform(1920, 1080, "Platform", 0);
	CJ_RENDERER renderer 	= CreateRenderer();
	CJ_InitRandom();
	GAME_HANDLER game 	= {};

    
	while(!glfwWindowShouldClose(platform.window) && running)
	{
        
		//////// PLATFORM ///////////
		UpdatePlatform(&platform);

		//////// GAME ///////////
		OnGameInit(&platform, &renderer, &game);
		OnGameUpdate(&platform, &renderer, &game);

		renderer.commands.n_entities_to_draw 	= game.entity_batch_count;
		renderer.commands.draw_offset[0] 		= game.sprite_box->id;
		renderer.commands.draw_offset[1] 		= game.collision_select_box->id;
		renderer.commands.draw_offset[2] 		= game.console.bkg_window->id;

		UpdateVBO_Entity(&renderer.vbo_sprite, &game);
        
		//////// RENDERER ///////////
		UpdateRenderer(&renderer, &platform);
        
        
		SwapBuffers(platform);
	}

	OnGameShutDown(&game);

    
    
    
	glfwTerminate();
    
    
    return 0;
}


