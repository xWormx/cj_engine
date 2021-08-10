
bool running = true;

#include "assert.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "cj_engine.h"
#include "glfw3native.h"
#include "commctrl.h"

GLenum err = 0;

struct TEXT_INFO
{
	V2f sentence_pos;
	V2f sentence_size;
	u32 n_files_in_dir;
};



bool CursorInsideText(CJ_PLATFORM *platform, TEXT_INFO ti);
bool CursorInsideEntity(CJ_PLATFORM *platform, ENTITY *e);
TEXT_INFO DrawText(CJ_PLATFORM *platform, CJ_RENDERER *renderer, u32 texture_index, char *str, V2f pos, V4f col);
void SwapOrderInIBO(CJ_RENDERER *renderer, ENTITY *e1, ENTITY *e2);
void UpdateVBO_Entity(CJ_VBO *vbo, GAME_HANDLER *game);

void GetDirContent(TEXT_INFO *ti, char *storage, char *dir, u32 max_filesz_name)
{
	ti->n_files_in_dir = 0;
	WIN32_FIND_DATA find_data 		= {};
	LARGE_INTEGER file_sz			= {};
	HANDLE hFind				= {};
    
	strcpy(storage, dir);
	strcat(storage, "/*");
	ti->n_files_in_dir++;
    
	hFind = FindFirstFile(storage, &find_data);
    
	if(hFind == INVALID_HANDLE_VALUE) 
		printf("couldn't find file in %s\n", storage);
	else
		storage += max_filesz_name;
    
	while(FindNextFile(hFind, &find_data) != 0)
	{
		if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(storage, find_data.cFileName);
			storage += max_filesz_name;
			ti->n_files_in_dir++;
            
		}
		else
		{
			char *dir_assets = "assets/";
			strcpy(storage, dir_assets);
			strcat(storage, find_data.cFileName);
			storage += max_filesz_name;
			ti->n_files_in_dir++;
		}
	}
    
}



ENTITY *PushEntity(GAME_HANDLER *game, u32 size)
{
	assert(game->entity_used + size <= game->entity_batch_count);
    
	ENTITY *entity 		= 0;
	CJ_VTX_QUAD *vtx	= 0;
    
	entity			= game->entity_batch + game->entity_used;;
	entity->id 		= game->entity_id;
	entity->ibo_id		= game->ibo_id;
    
    
    
	// vtx_id is for pushing 4 * CJ_VTX_QUAD
	game->vtx_id		+= 4 * size;
	game->ibo_id		+= 6 * size;
	game->entity_used 	+= size;
	game->entity_id		+= size;
	
    
	return entity;
}

void UpdateVBO_Entity(CJ_VBO *vbo, GAME_HANDLER *game)
{
	CJ_VTX_QUAD *pVertex = (CJ_VTX_QUAD*)vbo->base;
    
	assert(game->entity_used <= game->entity_batch_count);
    

	ENTITY *temp_sorted_batch = (ENTITY*)malloc( game->entity_used * sizeof(ENTITY) );
	memcpy(temp_sorted_batch, game->entity_batch, sizeof(ENTITY) * game->entity_used);

	for(u32 i = 1; i < game->entity_used; i++)
	{
		// sorting the entity_batch by position and collision_type before sending the data to the VBO
		u32 curr_index = i;
		for(u32 j = 1; j <= i; j++)
		{
			ENTITY *right = &temp_sorted_batch[curr_index];
			ENTITY *left = &temp_sorted_batch[i - j];
			u32 col_type_r = right->collision_type;
			u32 col_type_l = left->collision_type;


			if(right->pos.y > left->pos.y && 
			   col_type_r == collision_type_creature &&
			   col_type_l == collision_type_creature)
			{
				if(right->id > left->id)
				{
					ENTITY temp = {};
					memcpy(&temp, right, sizeof(ENTITY));
					memcpy(right, left, sizeof(ENTITY));
					memcpy(left, &temp, sizeof(ENTITY));
					curr_index--;
				}
			} // If rightmost element is of type ground and has an ID greater than the one to the left of it
			else if (col_type_r == collision_type_ground &&
				 col_type_l == collision_type_creature)
			{
				if(right->id > left->id)
				{
					ENTITY temp = {};
					memcpy(&temp, right, sizeof(ENTITY));
					memcpy(right, left, sizeof(ENTITY));
					memcpy(left, &temp, sizeof(ENTITY));
					curr_index--;
				}
			}

		}
		

	}


	// Updating the data that's sent to the VBO
	for(u32 i = 0; i < game->entity_used; i++)
	{
		V2f pos 	= temp_sorted_batch[i].pos;
		V2f size 	= temp_sorted_batch[i].size;
		V4f col 	= temp_sorted_batch[i].col;
		V2f tCoord_p	= temp_sorted_batch[i].texcoord_pos;
		V2f tCoord_sz	= temp_sorted_batch[i].texcoord_size;
        
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

	free(temp_sorted_batch);
    
}


bool CursorInsideText(CJ_PLATFORM *platform, TEXT_INFO ti)
{
	if(platform->cursor_pos.x > ti.sentence_pos.x &&
	   platform->cursor_pos.x < ti.sentence_size.x &&
	   platform->cursor_pos.y > ti.sentence_pos.y &&
	   platform->cursor_pos.y < ti.sentence_size.y)
		return true;
	else
		return false;
}

bool CursorInsideEntity(CJ_PLATFORM *platform, ENTITY *e)
{
	if(platform->cursor_pos.x > e->pos.x &&
	   platform->cursor_pos.x < e->pos.x + e->size.x &&
	   platform->cursor_pos.y > e->pos.y &&
	   platform->cursor_pos.y < e->pos.y + e->size.y)
		return true;
	else
		return false;
}


TEXT_INFO DrawText(CJ_PLATFORM *platform, CJ_RENDERER *renderer, u32 font_texture_index, char *str, V2f pos, V4f col)
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
	
	float biggest_h = 0.0f;
    
    
	TEXTUREINFO *tex_i 	= &renderer->texture_batch[font_texture_index];
	
	u32 n_chars 		= strlen(line);
	u32 size_vertices 	= 6 * n_chars * sizeof(CJ_VTX_QUAD);
	CJ_VTX_QUAD *vertices 	= (CJ_VTX_QUAD*)calloc(6 * n_chars, sizeof(CJ_VTX_QUAD));

	for(i32 i = 0; i < n_chars; i++)
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
        
	    vertices[(i * 6) + 0].pos 	= v2f(c_x, c_y);
	    vertices[(i * 6) + 0].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[(i * 6) + 0].tCoord = v2f(t_pos_x, t_pos_y);
        
	    vertices[(i * 6) + 1].pos 	= v2f(c_x, c_y + c_h);
	    vertices[(i * 6) + 1].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[(i * 6) + 1].tCoord = v2f(t_pos_x, t_pos_y + t_sz_y);
        
	    vertices[(i * 6) + 2].pos 	= v2f(c_x + c_w, c_y + c_h);
	    vertices[(i * 6) + 2].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[(i * 6) + 2].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);
        
	    vertices[(i * 6) + 3].pos 	= v2f(c_x, c_y);
	    vertices[(i * 6) + 3].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[(i * 6) + 3].tCoord = v2f(t_pos_x, t_pos_y);
        
	    vertices[(i * 6) + 4].pos 	= v2f(c_x + c_w, c_y + c_h);
	    vertices[(i * 6) + 4].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[(i * 6) + 4].tCoord = v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);
        
	    vertices[(i * 6) + 5].pos 	= v2f(c_x + c_w, c_y);
	    vertices[(i * 6) + 5].col 	= v4f(col.x, col.y, col.z, col.w);
	    vertices[(i * 6) + 5].tCoord = v2f(t_pos_x + t_sz_x, t_pos_y);
        
	    if(i == 0)
		    ti.sentence_pos = v2f(c_x, c_y);
        
	    if(c_y + c_h > biggest_h)
		    biggest_h = c_y + c_h;

	    renderer->n_chars_drawn++;
	}
    

	
	u8 *vbo_text_tracker = (u8*)(renderer->vbo_text.base + renderer->vbo_text_index_tracker);
	//assert(renderer->vbo_text_index_tracker <= renderer->vbo_text.size);
	memcpy(vbo_text_tracker, vertices, size_vertices);
	free(vertices);
	renderer->vbo_text_index_tracker += size_vertices;

	ti.sentence_size = v2f(c_x + c_w, biggest_h);
    
	return ti;
} 

//TEXT_INFO DrawTextSlow(CJ_PLATFORM *platform, CJ_RENDERER *renderer, u32 texture_index, char *str, V2f pos, V4f col)
//{
//	TEXT_INFO ti = {};
//    
//	char line[1024] = {};
//	strcpy(line, str);
//	
//	// xp & yp is baseline in stbtt_GetPackedQuad();
//	float yp = 0.0f;
//	float xp = 0.0f;
//	
//	float t_pos_x = 0.0f;
//	float t_pos_y = 0.0f;
//	float t_sz_x = 0.0f;
//	float t_sz_y = 0.0f;
//	
//	float c_x = 0.0f;
//	float c_y = 0.0f;
//	
//	float c_w = 0.0f;
//	float c_h = 0.0f;
//	
//	float bigges_h = 0.0f;
//    
//	u32 n_quads_written = 0;
//    
//	glBindVertexArray(renderer->VAO_font);
//	UseShaderProgram(&renderer->shader[2]);
//	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo_text.id);
//	glBindTexture(GL_TEXTURE_2D, renderer->texture_batch[texture_index].texture_id);
//    
//	CJ_VTX_QUAD *advPointer = (CJ_VTX_QUAD*)renderer->vbo_text.base;
//	TEXTUREINFO *tex_i = &renderer->texture_batch[texture_index];
//	
//	for(i32 i = 0; i < strlen(line); i++)
//	{
//	    stbtt_GetPackedQuad(tex_i->packed_char, 1024, 1024, line[i] - ' ', &xp, &yp, &tex_i->glyph_quad, 0);
//	    
//	    t_pos_x = tex_i->glyph_quad.s0;
//	    t_pos_y = 1.0f - tex_i->glyph_quad.t1;
//	    t_sz_x = tex_i->glyph_quad.s1 - tex_i->glyph_quad.s0;
//	    t_sz_y = (1.0f - tex_i->glyph_quad.t0) - (1.0f - tex_i->glyph_quad.t1);
//        
//	    c_x = pos.x + (tex_i->glyph_quad.x0)  / 1080.0f;
//	    c_y = pos.y + ((yp + (yp - tex_i->glyph_quad.y1))) / 960.0f;
//	    
//	    c_w = (tex_i->glyph_quad.x1 - tex_i->glyph_quad.x0)  / 1080.0f;
//	    c_h = (tex_i->glyph_quad.y1 - tex_i->glyph_quad.y0)  / 960.0f;
//        
//	    // 6 vertices for 1 quad i.e 1 character
//	    CJ_VTX_QUAD vertices[6] = {};
//	    vertices[0].pos 	= v2f(c_x, c_y);
//	    vertices[0].col 	= v4f(col.x, col.y, col.z, col.w);
//	    vertices[0].tCoord 	= v2f(t_pos_x, t_pos_y);
//        
//	    vertices[1].pos 	= v2f(c_x, c_y + c_h);
//	    vertices[1].col 	= v4f(col.x, col.y, col.z, col.w);
//	    vertices[1].tCoord 	= v2f(t_pos_x, t_pos_y + t_sz_y);
//        
//	    vertices[2].pos 	= v2f(c_x + c_w, c_y + c_h);
//	    vertices[2].col 	= v4f(col.x, col.y, col.z, col.w);
//	    vertices[2].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);
//        
//	    vertices[3].pos 	= v2f(c_x, c_y);
//	    vertices[3].col 	= v4f(col.x, col.y, col.z, col.w);
//	    vertices[3].tCoord 	= v2f(t_pos_x, t_pos_y);
//        
//	    vertices[4].pos 	= v2f(c_x + c_w, c_y + c_h);
//	    vertices[4].col 	= v4f(col.x, col.y, col.z, col.w);
//	    vertices[4].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y + t_sz_y);
//        
//	    vertices[5].pos 	= v2f(c_x + c_w, c_y);
//	    vertices[5].col 	= v4f(col.x, col.y, col.z, col.w);
//	    vertices[5].tCoord 	= v2f(t_pos_x + t_sz_x, t_pos_y);
//        
//	    memcpy(advPointer, vertices, sizeof(vertices));
//	    // offset means offest into the VBO ont the GPU (vbo_text) not the buffer on the CPU (advPointer)
//	    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vbo_text.size, (void*)renderer->vbo_text.base);
//        
//	    advPointer += 6;
//	    n_quads_written++;
//        
//	    if(i == 0)
//		    ti.sentence_pos = v2f(c_x, c_y);
//        
//	    if(c_y + c_h > bigges_h)
//		    bigges_h = c_y + c_h;
//	}
//    
//	ti.sentence_size = v2f(c_x + c_w, bigges_h);
//	glDrawArrays(GL_TRIANGLES, 0, n_quads_written * 6);
//    
//	return ti;
//} 

void OnGameInit(CJ_PLATFORM *platform, CJ_RENDERER *renderer, GAME_HANDLER *game)
{
	static bool is_init = false;
	if(!is_init)
	{
        
		game->entity_batch 		= (ENTITY*)calloc(1000, sizeof(ENTITY));
		game->entity_batch_count	= 1000 * sizeof(ENTITY);

		V2f tp = v2f((32.0f / 512.0f), (32.0f / 512.0f));
        
		game->current_chosen_atlas 			= PushEntity(game, 1);
		game->current_chosen_atlas->pos 		= v2f(0.0f, 1.2f); 
		game->current_chosen_atlas->size 		= v2f(0.6f, 0.8f); 
		game->current_chosen_atlas->col 		= v4f(1.0f, 1.0f, 1.0f, 1.0f); 
		game->current_chosen_atlas->texcoord_pos 	= v2f(0.0f, 0.0f);
		game->current_chosen_atlas->texcoord_size 	= v2f(1.0f, 1.0f);

		game->current_chosen_sprite 			= PushEntity(game, 1);
		game->current_chosen_sprite->pos 		= v2f(0.0f, 0.0f); 
		game->current_chosen_sprite->size 		= v2f(0.2f, 0.2f); 
		game->current_chosen_sprite->col 		= v4f(1.0f, 1.0f, 1.0f, 1.0f); 
		game->current_chosen_sprite->texcoord_pos 	= v2f(0.0f, 0.0f);
		game->current_chosen_sprite->texcoord_size 	= v2f(1.0f, 1.0f);


		V2f box_size 			= v2f((game->current_chosen_atlas->size.x / 16.0f), (game->current_chosen_atlas->size.y / 16.0f));
		game->sprite_box 		= PushEntity(game, 1);
		game->sprite_box->pos 		= v2f(0.0f, 2.0f - box_size.y); 
		game->sprite_box->size 		= box_size; 
		game->sprite_box->col 		= v4f(1.0f, 1.0f, 0.0f, 0.3f); 
		game->sprite_box->texcoord_pos 	= v2f(0.0f, 0.0f);
		game->sprite_box->texcoord_size = v2f(1.0f, 1.0f);

        
		game->player 			= PushEntity(game, 1);
		game->player->pos 		= v2f(1.0f, 1.7f); 
		game->player->size 		= v2f(0.06f, 0.1f); 
		game->player->col 		= v4f(1.0f, 1.0f, 1.0f, 1.0f); 
		game->player->texcoord_pos 	= v2f(0.0f * tp.x, 1.0f - (2.0f * tp.y));
		game->player->texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);
		game->player->collision_type	= collision_type_creature;
        
		game->monster 			= PushEntity(game, 1);
		game->monster->pos 		= v2f(1.0f, 1.0f); 
		game->monster->size 		= v2f(0.2f, 0.2f); 
		game->monster->col 		= v4f(1.0f, 1.0f, 1.0f, 0.8f); 
		game->monster->texcoord_pos 	= v2f(3.0f * tp.x, 1.0f - (1.0f * tp.y));
		game->monster->texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);
		game->monster->collision_type	= collision_type_creature;

        
		u32 tilemap_w 			= 10;
		u32 tilemap_h 			= 10;
		V2f start_pos 			= v2f(0.5f, 0.5f);
		V2f tile_size 			= v2f(0.11f, 0.11f);

		game->tilemap.entity 		= PushEntity(game, tilemap_w * tilemap_h);
		game->tilemap.w			= tilemap_w;
		game->tilemap.h			= tilemap_h;

        
        
		for(u32 y = 0; y < tilemap_h; y++)
		{
			for(u32 x = 0; x < tilemap_w; x++)
			{
				u32 index = x + (y * tilemap_w);
                
				//float randW = CJ_RandFRange(0.011f, 0.074f, CJ_RAND_DONT_INCLUDE);
				//float randH = CJ_RandFRange(0.011f, 0.074f, CJ_RAND_DONT_INCLUDE);
				//printf("randW: %f\n", randW);
				//printf("randH: %f\n", randH);

				float separation = 0.002f;
                
				V2f pos 	= v2f(start_pos.x + (x * tile_size.x), start_pos.y + (y * tile_size.y));
				V2f size 	= v2f(tile_size.x - separation, tile_size.y - separation);
				V4f col 	= v4f(1.0f, 1.0f, 1.0f, 1.0f);
                
				game->tilemap.entity[index].pos = pos;
				game->tilemap.entity[index].size = size;
				game->tilemap.entity[index].col = col;
                
				if(x == 0 || y == 0 || x == tilemap_w - 1 || y == tilemap_h - 1)
				{
					game->tilemap.entity[index].texcoord_pos 	= v2f(0.0f * tp.x, 1.0f - (2.0f * tp.y));
					game->tilemap.entity[index].texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);
					game->tilemap.entity[index].collision_type 	= collision_type_edge;
				}
				else
				{
					game->tilemap.entity[index].texcoord_pos 	= v2f(2.0f * tp.x, 1.0f - (1.0f * tp.y));
					game->tilemap.entity[index].texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);
					game->tilemap.entity[index].collision_type 	= collision_type_ground;
				}

				game->tilemap.entity[index].ibo_id = game->tilemap.entity[0].ibo_id + (6 * index);
				game->tilemap.entity[index].id = game->tilemap.entity[0].id + index;
                
                
			}
            
		}
        
        
		game->camera 		= PushEntity(game, 1);
		game->camera->pos 	= v2f(0.0f, 0.0f);


        
        
		is_init = true;
	}
    
}

void OnGameUpdate(CJ_PLATFORM *platform, CJ_RENDERER *renderer, GAME_HANDLER *game)
{
    
	renderer->vbo_text_index_tracker 	= 0;
	renderer->n_chars_drawn			= 0;

	memset(renderer->vbo_text.base, 0, renderer->vbo_text.size);

	if(KeyPressed(platform, GLFW_KEY_ESCAPE))
	{
		running = false;
	}

	if(KeyPressedOnce(GLFW_KEY_COMMA))
	{
		if(game->state == game_state_main_menu) game->state = game_state_console;
		else if(game->state == game_state_console) game->state = game_state_main_menu;
	}
	
	/////////////////////////////////// CONSOLE TEXT ///////////////////////////////////////////////

	

	if(game->state == game_state_console)
	{
		game->console.alpha_mul = 0.3f;

		for(i32 y = 0; y < 10; y++)
		{
			DrawText(platform, renderer, 2, (char*)game->console.str_base, v2f(0.0f, y * 0.08f), COLOR_PURPLE);

		}

		char buff[100] = {};
		sprintf(buff, "chars drawn: %d", renderer->n_chars_drawn);
		DrawText(platform, renderer, 2, buff, v2f(1.0f, 0.1), COLOR_RED);
	}
	else
	{
		game->console.alpha_mul = 1.0f;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
    
	float mov = 0.003f;
    
	game->camera->vel = v2f(0.f, 0.f);
	if(KeyPressed(platform, GLFW_KEY_UP))
	{
		game->camera->vel.y = mov;
	}
	if(KeyPressed(platform, GLFW_KEY_DOWN))
	{
		game->camera->vel.y = -mov;
	}
	if(KeyPressed(platform, GLFW_KEY_RIGHT))
	{
		game->camera->vel.x = mov;
	}
	if(KeyPressed(platform, GLFW_KEY_LEFT))
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
    
	if(game->player->pos.y > clamp_top)
	{
		game->player->pos.y = clamp_top;
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
		if(KeyPressed(platform, GLFW_KEY_SPACE))
		{
			monster->pos.x += 0.0101f * direction.x;
			monster->pos.y += 0.0101f * direction.y;
		}
	}

	// Only checking the tile entities in the entity batch!
	u32 start_entity_id = game->tilemap.entity->id;
	u32 n_tiles = (game->tilemap.w  * game->tilemap.h);
	for(u32 i = start_entity_id; i < start_entity_id + n_tiles; i++)
	{
		V2f pos = game->entity_batch[i].pos;
		V2f size = game->entity_batch[i].size;
		pos.x += size.x / 2.0f;
		pos.y += size.y / 2.0f;
		char index[3] = {};
		sprintf(index, "%d", i - start_entity_id);
		DrawText(platform, renderer, 2, index, pos, COLOR_GREEN);
		if(clamped)
		{
			game->entity_batch[i].pos -= game->camera->vel;
		}


		if(CursorInsideEntity(platform, &game->entity_batch[i]))
		{
			if(MousePressed(platform, GLFW_MOUSE_BUTTON_LEFT))
			{
				DrawText(platform, renderer, 2, "LEFT MOUSE PRESSED", v2f(0.0f, 0.8f), COLOR_GREEN);
				V2f tp = v2f((32.0f / 512.0f), (32.0f / 512.0f));
				game->entity_batch[i].texcoord_pos 	= game->current_chosen_sprite->texcoord_pos;
				game->entity_batch[i].texcoord_size 	= game->current_chosen_sprite->texcoord_size;
				if(KeyPressed(platform, 'C'))
				{
					game->entity_batch[i].collision_type = collision_type_structure;
					game->entity_batch[i].col = v4f(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else if(KeyPressed(platform, 'G'))
				{
					game->entity_batch[i].collision_type = collision_type_ground;
					game->entity_batch[i].col = COLOR_WHITE;
				}
						
			}


		}

		COLLISION_INFO ci = Collision_SAT_ENTITY_ENTITY(game->player, &game->entity_batch[i]);
		if(ci.collided && game->entity_batch[i].collision_type == collision_type_structure)
		{
			// add the minimum translation vector to the position.
			game->player->pos += ci.mtv;
		}
		ci = Collision_SAT_ENTITY_ENTITY(game->monster, &game->entity_batch[i]);
		if(ci.collided && game->entity_batch[i].collision_type == collision_type_structure)
		{
			// add the minimum translation vector to the position.
			game->monster->pos += ci.mtv;
		}

		game->entity_batch[i].col.w = game->console.alpha_mul;
		


	}

	ENTITY *sprite_box 	= game->sprite_box;
	ENTITY *atlas 		= game->current_chosen_atlas;
	ENTITY *chosen_sprite 	= game->current_chosen_sprite;
	float tile_atlas_w 	= game->current_chosen_atlas->size.x;
	float tile_atlas_h 	= 2.0f - game->current_chosen_atlas->size.y;
	V2f tp 			= v2f((32.0f / 512.0f), (32.0f / 512.0f));

	if(platform->cursor_pos.x <= tile_atlas_w &&
	   platform->cursor_pos.y >= tile_atlas_h)
	{
		// multiply mousecoord's and atlas size so we can use integer division to get integers 
		// for dividing cursor pos by 16 so we get how many steps of 16th the cursor pos are into the atlas.
		float n_cpx = platform->cursor_pos.x * platform->win_w;
		float n_cpy = (platform->cursor_pos.y - (atlas->pos.y)) * platform->win_h;

		float n_atlas_szx = atlas->size.x * platform->win_w;
		float n_atlas_szy = atlas->size.y * platform->win_h;



		u32 scale_box_x = (u32)n_atlas_szx / 16;
		u32 scale_box_y = (u32)n_atlas_szy / 16;
		u32 x_factor = (u32)n_cpx / scale_box_x;
		u32 y_factor = (u32)n_cpy / scale_box_y;

		if(x_factor < 16 && y_factor < 16)
		{
			sprite_box->col = v4f(1.0f, 1.0f, 0.0f, 0.8f);
			sprite_box->pos.x = game->current_chosen_atlas->pos.x + (x_factor * sprite_box->size.x);
			sprite_box->pos.y = game->current_chosen_atlas->pos.y + (y_factor * sprite_box->size.y);

			if(MousePressed(platform, GLFW_MOUSE_BUTTON_LEFT))
			{
				sprite_box->col = COLOR_GREEN;
				char buffer[256] = {};
				sprintf(buffer, "tile selected(%d/%d)", x_factor, y_factor);
				DrawText(platform, renderer, 2, buffer, v2f(0.0f, 0.3f), COLOR_GREEN);

				chosen_sprite->texcoord_pos 	= v2f(x_factor * tp.x, -tp.y + ((1.0f + y_factor) * tp.y));
				chosen_sprite->texcoord_size 	= v2f(1.0f * tp.x, 1.0f * tp.y);
			}
		}

	}
	else
	{
		sprite_box->col = v4f(0.0f, 0.0f, 0.0f, 0.0f);
		
	}

    
	char buffer[256] = {};
	sprintf(buffer, "game.player->pos: (%f / %f)", game->player->pos.x, game->player->pos.y);
    
	static V4f tCol 	= COLOR_ORANGE;
	static V4f tCol_save 	= COLOR_GREEN;
	static V4f tCol_select 	= COLOR_GREEN;
	// can create ti before gameloop to prevent setting the color after the text is rendered.
	TEXT_INFO ti = {};
	ti = DrawText(platform, renderer, 2, buffer, v2f(0.2f, 1.7f), tCol);
	if(CursorInsideText(platform, ti)) 	{ tCol = COLOR_RED; } else { tCol = COLOR_ORANGE; }
    
	sprintf(buffer, "time since start: (%f)", GetTime());
	ti = DrawText(platform, renderer, 2, buffer, v2f(1.2f, 1.8f), COLOR_GREEN);

	sprintf(buffer, "time per frame: (%f)", platform->time_per_frame);
	ti = DrawText(platform, renderer, 2, buffer, v2f(1.2f, 1.7f), COLOR_GREEN);
	
	sprintf(buffer, "platform.cursor_pos: (%f / %f)", platform->cursor_pos.x, platform->cursor_pos.y);
	DrawText(platform, renderer, 2, buffer, v2f(0.8f, 0.3f), v4f(1.0f, 1.0f, 1.0f, 1.0f));
	sprintf(buffer, "platform.cursor_pos_int: (%d / %d)", platform->cursor_pos_int.x, platform->cursor_pos_int.y);
	DrawText(platform, renderer, 2, buffer, v2f(0.8f, 0.2f), v4f(1.0f, 1.0f, 1.0f, 1.0f));
    
	ti = DrawText(platform, renderer, 2, "Select atlas", v2f(1.7f, 1.3f), tCol_select);
	if(CursorInsideText(platform, ti)) 	{ tCol_select = COLOR_RED; } else { tCol_select = COLOR_GREEN; }
    
    
	ti = DrawText(platform, renderer, 2, "Save map", v2f(1.7f, 1.2f), tCol_save);
	static bool b_save_as = false;
	if(CursorInsideText(platform, ti)) 	
	{ 
		tCol_save = COLOR_RED; 
		if(MousePressedOnce(GLFW_MOUSE_BUTTON_LEFT))
		{
			ti = DrawText(platform, renderer, 2, "LEFT MOUSE PRESSED", v2f(0.7f, 1.3f), tCol_select);
			b_save_as = true;
		}
	} else 
	{ 
		tCol_save = COLOR_GREEN; 
	}

	if(b_save_as)
	{

		char save_as[256] = "Save As: ";
		static char file_name[256] = {};
		if(KeyPressedOnce(GLFW_KEY_ENTER))
		{
			// Creating a new file if it doesn't already exist
			if(file_name[0] != '\0')
			{
				FILE *f_handle = fopen(file_name, "ab+");
				fclose(f_handle);
				b_save_as = false;
			}

		}
		else
		{
			sprintf(file_name, (char*)game->console.str_base);
			strcat(save_as, file_name);
			ti = DrawText(platform, renderer, 2, save_as, v2f(0.7f, 1.3f), tCol_select);
		}
	}
    
    
    
	char dir_content[24][1024] = {};
	GetDirContent(&ti, dir_content[0], "assets", 1024);
	u32 files_in_dir = ti.n_files_in_dir;
	static bool init = false;
	if(!init)
	{
		init = true;
		for(u32 i = 0; i < ti.n_files_in_dir; i++)
		{
			printf("dir_content[%d]: %s\n", i, dir_content[i]);
		}
	}
    
	static V4f text_dir_cont_col[24] = {};
	static V4f yes_no[2] = {};
	static bool bLoad_texture = false;
	
	static char file_to_choose[256] = {};
	for(u32 i = 0; i < files_in_dir; i++)
	{
		ti = DrawText(platform, renderer, 2, dir_content[i], v2f(1.6f, 1.8f - i * 0.1f), text_dir_cont_col[i]);
		if(CursorInsideText(platform, ti)) 	
		{ 
			text_dir_cont_col[i] = COLOR_RED; 
			if(MousePressedOnce(GLFW_MOUSE_BUTTON_LEFT) && !bLoad_texture)
			{
                		strcpy(file_to_choose, dir_content[i]);
                		bLoad_texture = true;
			}
            
		} 
		else 
		{ 
			text_dir_cont_col[i] = COLOR_GREEN; 
		}
        
		if(bLoad_texture)
		{
			ti = DrawText(platform, renderer, 2, "Load texture?", v2f(0.7f, 1.0f), COLOR_GREEN);
			static V4f yes_col = {};
			ti = DrawText(platform, renderer, 2, "Yes", v2f(0.7f, 0.9f), yes_col);
			if(CursorInsideText(platform, ti)) 	
			{ 
				yes_col = COLOR_RED; 
				if(MousePressedOnce(GLFW_MOUSE_BUTTON_LEFT))
				{
					LoadTexture(&renderer->texture_batch[0], file_to_choose, LOAD_IMAGE, 0);
					bLoad_texture = false;
				}
                
			} 
			else if(KeyPressedOnce(GLFW_KEY_ENTER))
			{
				LoadTexture(&renderer->texture_batch[0], file_to_choose, LOAD_IMAGE, 0);
				bLoad_texture = false;
			}
			else
			{ 
				yes_col = COLOR_GREEN; 
			}
            
			static V4f no_col = {};
			ti = DrawText(platform, renderer, 2, "No", v2f(0.8f, 0.9f), no_col);
			if(CursorInsideText(platform, ti)) 	
			{ 
				no_col = COLOR_RED; 
				if(MousePressedOnce(GLFW_MOUSE_BUTTON_LEFT))
				{
					bLoad_texture = false;
				}
                
			} 
			else 
			{ 
				no_col = COLOR_GREEN; 
			}
		}
        
	}





    
}




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


