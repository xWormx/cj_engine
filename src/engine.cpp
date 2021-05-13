
// TODO: DEBUG code for OpenGL setup 
// TODO: Write a KeyPressedOnce();
// TODO: How to change text size???


bool running = true;

#include "assert.h"
#include "cj_engine.h"

GLenum err = 0;


bool init = false;


struct TEXT_INFO
{
	V2f sentence_pos;
	V2f sentence_size;
};

struct TILEMAP
{
	u32 w;
	u32 h;
	ENTITY *entity;
};

struct GAME_HANDLER
{
	ENTITY *entity_batch;
	u32 entity_batch_count;
	u32 entity_used;
	u32 entity_id;

	CJ_VTX_QUAD *pVertex;
	u32 vtx_id;

	TILEMAP tilemap;

	ENTITY *player;
	ENTITY *monster;

	ENTITY *camera;


};



bool CursorInsideText(CJ_PLATFORM platform, TEXT_INFO ti);
TEXT_INFO DrawText(CJ_PLATFORM *platform, RENDERER *renderer, u32 texture_index, char *str, V2f pos, V4f col);



ENTITY *PushEntity(GAME_HANDLER *game, u32 size)
{
	assert(game->entity_used + size <= game->entity_batch_count);

	ENTITY *entity 		= 0;
	CJ_VTX_QUAD *vtx	= 0;

	entity			= game->entity_batch + game->entity_used;;
	entity->id 		= game->entity_id;

	vtx			= game->pVertex + game->vtx_id;


	// vtx_id is for pushing 4 * CJ_VTX_QUAD
	game->vtx_id		+= 4 * size;
	game->entity_used += size;
	game->entity_id++;
	

	return entity;
}

void UpdateVBO_Entity(CJ_VBO *vbo, GAME_HANDLER *game)
{
	CJ_VTX_QUAD *pVertex = (CJ_VTX_QUAD*)vbo->base;

	assert(game->entity_used <= game->entity_batch_count);

	for(u32 i = 0; i < game->entity_used; i++)
	{
		V2f pos 	= game->entity_batch[i].pos;
		V2f size 	= game->entity_batch[i].size;
		V4f col 	= game->entity_batch[i].col;
		V2f tCoord_p	= game->entity_batch[i].texcoord_pos;
		V2f tCoord_sz	= game->entity_batch[i].texcoord_size;

		pVertex[(i * 4) + 0].pos = v2f(pos.x, pos.y);
		pVertex[(i * 4) + 1].pos = v2f(pos.x, pos.y + size.y);
		pVertex[(i * 4) + 2].pos = v2f(pos.x + size.x, pos.y + size.y);
		pVertex[(i * 4) + 3].pos = v2f(pos.x + size.x, pos.y);

		pVertex[(i * 4) + 0].col = col;
		pVertex[(i * 4) + 1].col = col;
		pVertex[(i * 4) + 2].col = col;
		pVertex[(i * 4) + 3].col = col;

		pVertex[(i * 4) + 0].tCoord = v2f(tCoord_p.x, tCoord_p.y);
		pVertex[(i * 4) + 1].tCoord = v2f(tCoord_p.x, tCoord_p.y + tCoord_sz.y);
		pVertex[(i * 4) + 2].tCoord = v2f(tCoord_p.x + tCoord_sz.x, tCoord_p.y + tCoord_sz.y);
		pVertex[(i * 4) + 3].tCoord = v2f(tCoord_p.x + tCoord_sz.y, tCoord_p.y);

	}

}
void OnGameInit(CJ_PLATFORM *platform, RENDERER *renderer, GAME_HANDLER *game)
{
	if(!init)
	{

		game->entity_batch 		= (ENTITY*)calloc(1000, sizeof(ENTITY));
		game->entity_batch_count	= 1000 * sizeof(ENTITY);
		game->pVertex 			= (CJ_VTX_QUAD*)renderer->vbo_sprite.base;

		u32 tilemap_w = 10;
		u32 tilemap_h = 10;
		V2f start_pos = v2f(0.5f, 0.5f);
		V2f tile_size = v2f(0.11f, 0.11f);
		game->tilemap.entity = PushEntity(game, tilemap_w * tilemap_h);
		game->tilemap.w = tilemap_w;
		game->tilemap.h = tilemap_h;
	

		V2f tp = v2f((32.0f / 512.0f), (32.0f / 512.0f));
		for(u32 y = 0; y < tilemap_h; y++)
		{
			for(u32 x = 0; x < tilemap_w; x++)
			{
				u32 index = x + (y * tilemap_w);

				float randW = CJ_RandFRange(0.011f, 0.074f, CJ_RAND_DONT_INCLUDE);
				float randH = CJ_RandFRange(0.011f, 0.074f, CJ_RAND_DONT_INCLUDE);
				float separation = 0.002f;
				printf("randW: %f\n", randW);
				printf("randH: %f\n", randH);

				V2f pos = v2f(start_pos.x + (x * tile_size.x), start_pos.y + (y * tile_size.y));
				V2f size = v2f(tile_size.x - separation, tile_size.y - separation);
				V4f col = v4f(1.0f, 1.0f, 1.0f, 1.0f);

				game->tilemap.entity[index].pos = pos;
				game->tilemap.entity[index].size = size;
				game->tilemap.entity[index].col = col;

				game->tilemap.entity[index].texcoord_pos 	= v2f(0.0f * tp.x, 1.0f - (1.0f * tp.y));
				game->tilemap.entity[index].texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);


			}

		}

		game->player 			= PushEntity(game, 1);
		game->player->pos 		= v2f(1.0f, 1.7f); 
		game->player->size 		= v2f(0.03f, 0.1f); 
		game->player->col 		= v4f(1.0f, 1.0f, 1.0f, 1.0f); 
		game->player->texcoord_pos 	= v2f(0.0f, 0.0f);
		game->player->texcoord_size 	= v2f(1.0f, 1.0f);

		game->monster 			= PushEntity(game, 1);
		game->monster->pos 		= v2f(1.0f, 1.0f); 
		game->monster->size 		= v2f(0.2f, 0.2f); 
		game->monster->col 		= v4f(1.0f, 1.0f, 1.0f, 0.8f); 
		game->monster->texcoord_pos 	= v2f(3.0f * tp.x, 1.0f - (1.0f * tp.y));
		game->monster->texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);

		game->camera = PushEntity(game, 1);
		game->camera->pos = v2f(0.0f, 0.0f);




		init = true;
	}

}

void OnGameUpdate(CJ_PLATFORM *platform, RENDERER *renderer, GAME_HANDLER *game)
{
	if(GetKey(platform, GLFW_KEY_ESCAPE))
	{
		running = false;
	}

	float mov = 0.003f;

	game->camera->vel = v2f(0.f, 0.f);
	if(GetKey(platform, GLFW_KEY_UP))
	{
		game->camera->vel.y = mov;
	}
	if(GetKey(platform, GLFW_KEY_DOWN))
	{
		game->camera->vel.y = -mov;
	}
	if(GetKey(platform, GLFW_KEY_RIGHT))
	{
		game->camera->vel.x = mov;
	}
	if(GetKey(platform, GLFW_KEY_LEFT))
	{
		game->camera->vel.x = -mov;
	}


	game->player->pos += game->camera->vel;

	bool clamped = false;
	TILEMAP *tilemap = &game->tilemap;
	float clamp_left 	= tilemap->entity[0].pos.x + tilemap->entity[0].size.x;
	float clamp_right 	= tilemap->entity[tilemap->w - 1].pos.x;
	float clamp_bottom 	= tilemap->entity[0].pos.y + tilemap->entity[0].size.y;
	float clamp_top 	= tilemap->entity[tilemap->w * tilemap->h - 1].pos.y;

	if(game->player->pos.x <= clamp_left)
	{
		game->player->pos.x = clamp_left;
		game->player->pos -= game->camera->vel;
		clamped = true;
	}
	if(game->player->pos.x + game->player->size.x >= clamp_right)
	{
		game->player->pos.x = clamp_right - game->player->size.x;
		game->player->pos -= game->camera->vel;
		clamped = true;

	}
	if(game->player->pos.y < clamp_bottom)
	{
		game->player->pos.y = clamp_bottom;
		game->player->pos -= game->camera->vel;
		clamped = true;
	}

	if(game->player->pos.y + game->player->size.y > clamp_top)
	{
		game->player->pos.y = clamp_top - game->player->size.y;
		game->player->pos -= game->camera->vel;

		clamped = true;
	}

	ENTITY *monster = game->monster;
	ENTITY *player = game->player;

	if(monster->pos.x != player->pos.x || monster->pos.y != player->pos.y)
	{
		V2f direction = {};
		direction.x = player->pos.x - monster->pos.x;
		direction.y = player->pos.y - monster->pos.y;
		direction = Unit_v2f(direction);

		monster->pos.x += 0.001f * direction.x;
		monster->pos.y += 0.001f * direction.y;
	}
	if(clamped)
	{
		for(u32 i = game->tilemap.entity->id; i < game->tilemap.w  * game->tilemap.h; i++)
		{
			game->entity_batch[i].pos -= game->camera->vel;
		}
	}
		char buffer[256] = {};
		sprintf(buffer, "game.player->pos: (%f / %f)", game->player->pos.x, game->player->pos.y);

		static V4f tCol = COLOR_GREEN;
		// can create ti before gameloop to prevent setting the color after the text is rendered.
		TEXT_INFO ti = DrawText(platform, renderer, 1, buffer, v2f(0.2f, 1.7f), tCol);

		if(CursorInsideText(*platform, ti))
		{
			tCol = COLOR_RED;
		}
		else
		{
			tCol = COLOR_GREEN;
		}
		
		sprintf(buffer, "platform.cursor_pos: (%f / %f)", platform->cursor_pos.x, platform->cursor_pos.y);
		DrawText(platform, renderer, 1, buffer, v2f(0.4f, 0.3f), v4f(1.0f, 1.0f, 1.0f, 1.0f));


}

int main()
{

	CJ_PLATFORM platform 	= CreatePlatform(640, 480, "Platform", 0);
	RENDERER renderer 	= CreateRenderer();
	CJ_InitRandom();
	GAME_HANDLER game 	= {};

	while(!glfwWindowShouldClose(platform.window) && running)
	{

		GetWindowSize(&platform);
		GetCursorPos(&platform);
		glViewport(0, 0, platform.win_w, platform.win_h);

		glfwPollEvents();

		RenderClear(v4f(0.1f, 0.1f, 0.1f, 1.0f));

		OnGameInit(&platform, &renderer, &game);
		OnGameUpdate(&platform, &renderer, &game);


		glBindVertexArray(renderer.VAO_texture_quad);
		glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_sprite.id);

		UpdateVBO_Entity(&renderer.vbo_sprite, &game);
		glBufferSubData(GL_ARRAY_BUFFER, 0, renderer.vbo_sprite.size, (void*)renderer.vbo_sprite.base);

		glBindTexture(GL_TEXTURE_2D, renderer.texture_batch[0].texture_id);

		DrawElements(&renderer, 1, 0, game.entity_batch_count, 0);
	


		SwapBuffers(platform);
	}



	glfwTerminate();
    
    
    return 0;
}

bool CursorInsideText(CJ_PLATFORM platform, TEXT_INFO ti)
{
	if(platform.cursor_pos.x > ti.sentence_pos.x &&
	   platform.cursor_pos.x < ti.sentence_size.x &&
	   platform.cursor_pos.y > ti.sentence_pos.y &&
	   platform.cursor_pos.y < ti.sentence_size.y)
		return true;
	else
		return false;
}

TEXT_INFO DrawText(CJ_PLATFORM *platform, RENDERER *renderer, u32 texture_index, char *str, V2f pos, V4f col)
{
	TEXT_INFO ti = {};

	char line[1024] = {};
	strcpy(line, str);
	
	// xp & yp is baseline in stbtt_GetPackedQuad();
	float yp = 0.0f;
	float xp = 0.0f;
	
	float t_pos_x = 0.0f;
	float t_pos_y = 0.0f;
	float t_sz_x = 0.0f;
	float t_sz_y = 0.0f;
	
	float c_x = 0.0f;
	float c_y = 0.0f;
	
	float c_w = 0.0f;
	float c_h = 0.0f;
	
	u32 n_quads_written = 0;

	glBindVertexArray(renderer->VAO_font);
	UseShaderProgram(&renderer->shader[2]);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_text.id);
	glBindTexture(GL_TEXTURE_2D, renderer->texture_batch[texture_index].texture_id);

	CJ_VTX_QUAD *advPointer = (CJ_VTX_QUAD*)renderer->vbo_text.base;
	TEXTUREINFO *tex_i = &renderer->texture_batch[texture_index];
	
	for(i32 i = 0; i < strlen(line); i++)
	{
	    stbtt_GetPackedQuad(tex_i->packed_char, 1024, 1024, line[i] - ' ', &xp, &yp, &tex_i->glyph_quad, 0);
	    
	    t_pos_x = tex_i->glyph_quad.s0;
	    t_pos_y = 1.0f - tex_i->glyph_quad.t1;
	    t_sz_x = tex_i->glyph_quad.s1 - tex_i->glyph_quad.s0;
	    t_sz_y = (1.0f - tex_i->glyph_quad.t0) - (1.0f - tex_i->glyph_quad.t1);

	    c_x = pos.x + (tex_i->glyph_quad.x0)  / 1080.0f;
	    c_y = pos.y + ((yp + (yp - tex_i->glyph_quad.y1))) / 960.0f;
	    
	    c_w = (tex_i->glyph_quad.x1 - tex_i->glyph_quad.x0)  / 1080.0f;
	    c_h = (tex_i->glyph_quad.y1 - tex_i->glyph_quad.y0)  / 960.0f;

	    CJ_VTX_QUAD vertices[6] = {};
	    vertices[0].pos 	= v2f(c_x, c_y);
	    vertices[0].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[0].tCoord 	= v2f(t_pos_x, t_pos_y);

	    vertices[1].pos 	= v2f(c_x, c_y + c_h);
	    vertices[1].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[1].tCoord 	= v2f(t_pos_x, t_pos_y + t_sz_y);

	    vertices[2].pos 	= v2f(c_x + c_w, c_y + c_h);
	    vertices[2].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[2].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);

	    vertices[3].pos 	= v2f(c_x, c_y);
	    vertices[3].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[3].tCoord 	= v2f(t_pos_x, t_pos_y);

	    vertices[4].pos 	= v2f(c_x + c_w, c_y + c_h);
	    vertices[4].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[4].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);

	    vertices[5].pos 	= v2f(c_x + c_w, c_y);
	    vertices[5].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[5].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y);
	
	    memcpy(advPointer, vertices, sizeof(vertices));
	    // offset means offest into the VBO ont the GPU (vbo_text) not the buffer on the CPU (advPointer)
	    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_text.size, (void*)renderer->vbo_text.base);

	    advPointer += 6;
	    n_quads_written++;

	    if(i == 0)
		    ti.sentence_pos = v2f(c_x, c_y);

	}

	// TODO: Could check for the largest size instead of the size of the last characterbox.
	ti.sentence_size = v2f(c_x + c_w, c_y + c_h);
	glDrawArrays(GL_TRIANGLES, 0, n_quads_written * 6);

	return ti;
} 





