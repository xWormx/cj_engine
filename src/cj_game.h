#ifndef _GAME_H
#define _GAME_H

enum INIT_STATE
{
        init_not_done, init_done
};

struct GAME
{

	V2f win_dim;
	V2f mousePos {};
	bool initialized;
	
	float dts;
	float ms_per_frame;
	

	
};

// **********	FUNC DECL	************************


void InitGame(GAME *game, CJ_RENDERER *renderer);
void UpdateGame(GAME *game, CJ_RENDERER *renderer, TEXTUREINFO *texture_batch);


void DefaultControl_ARROWS(GAME *game, ENTITY *e);
void DefaultControl_WASD(GAME *game, ENTITY *e);





u32 CreateText(CJ_RENDERER *renderer, TEXTUREINFO tex_i, i32 quad_type, char *str, V2f pos, V4f col);


// **********	FUNC DEFINITIONS	************************
void InitGame(GAME *game, CJ_RENDERER *renderer)
{
}

void UpdateGame(GAME *game, CJ_RENDERER *renderer, TEXTUREINFO *texture_batch)
{

    	InitGame(game, renderer);

	
}



u32 CreateText(CJ_RENDERER *renderer, TEXTUREINFO tex_i, i32 quad_type, char *str, V2f pos, V4f col)
{
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
    
    glBindTexture(GL_TEXTURE_2D, tex_i.texture_id);
    
    u32 last_quad_index = 0;
    u32 last_text_q = 0;
    u32 n_quads_written = 0;

    for(i32 i = 0; i < strlen(line); i++)
    {
        stbtt_GetPackedQuad(tex_i.packed_char, 1024, 1024, line[i] - ' ', &xp, &yp, &tex_i.glyph_quad, 0);
        
        t_pos_x = tex_i.glyph_quad.s0;
        t_pos_y = 1.0f - tex_i.glyph_quad.t1;
        t_sz_x = tex_i.glyph_quad.s1 - tex_i.glyph_quad.s0;
        t_sz_y = (1.0f - tex_i.glyph_quad.t0) - (1.0f - tex_i.glyph_quad.t1);
        
        // The glyph_quad.x0 & glyph_quad.y0 should be the the advancing positions in x/y, yp is the baseline which has to used when calculating the char y_pos 
        c_x = pos.x + (tex_i.glyph_quad.x0)  / renderer->win_dim.x;
        c_y = pos.y + ((yp + (yp - tex_i.glyph_quad.y1))) / renderer->win_dim.y;
        
        c_w = (tex_i.glyph_quad.x1 - tex_i.glyph_quad.x0)  / renderer->win_dim.x;
        c_h = (tex_i.glyph_quad.y1 - tex_i.glyph_quad.y0)  / renderer->win_dim.y;
        
	last_text_q = InitRenderQuad(renderer, quad_type, v2f(c_x, c_y), v2f(c_w, c_h), v4f(col.x, col.y, col.z, col.w), v2f(t_pos_x, t_pos_y), v2f(t_sz_x, t_sz_y));
	n_quads_written++;
    }

    //return last_quad_index;
    return last_text_q;
} 




#endif
