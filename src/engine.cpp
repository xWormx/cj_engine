
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

		//////// CONSOLE ///////////
		u32 max_console_chars = 10;
		//BeginConsole(&game.console, renderer.max_chars_to_render);
		BeginConsole(&game.console, max_console_chars);
		UpdateConsole(&game.console);


		//////// GAME ///////////
		OnGameInit(&platform, &renderer, &game);
		OnGameUpdate(&platform, &renderer, &game);



		glViewport(0, 0, platform.win_w, platform.win_h);
		RenderClear(v4f(0.011f, 0.011f, 0.011f, 1.0f));
        	


		glBindVertexArray(renderer.VAO_texture_quad);
		glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_sprite.id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer.vbo_sprite.size, (void*)renderer.vbo_sprite.base);

		DrawElements(&renderer, 1, 0, game.entity_batch_count, 0);
		DrawElements(&renderer, 0, 0, 1, game.sprite_box->id);

		UpdateVBO_Entity(&renderer.vbo_sprite, &game);
        	
		// RENDERING TEXT ///
		UseShaderProgram(&renderer.shader[2]);
		glBindVertexArray(renderer.VAO_font);
		glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_text.id);
		glBindTexture(GL_TEXTURE_2D, renderer.texture_batch[2].texture_id);

		// offset means offest into the VBO ont the GPU (vbo_text) not the buffer on the CPU
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer.vbo_text.size, (void*)renderer.vbo_text.base);
		glDrawArrays(GL_TRIANGLES, 0, renderer.max_chars_to_render * 6);
        	
		// RENDERING TEXT END ///
        
	//	//////// RENDERER ///////////
	//	UpdateVBO_Entity(&renderer.vbo_sprite, &game);
	//	UpdateRenderer(&renderer, &platform, &game);
        
        
		SwapBuffers(platform);
	}

	EndConsole(&game.console);
    
    
    
	glfwTerminate();
    
    
    return 0;
}


