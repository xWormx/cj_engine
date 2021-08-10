#ifndef CJ_GAME_H
#define CJ_GAME_H


struct TILEMAP
{
	u32 w;
	u32 h;
	ENTITY *entity;
};

enum GAME_STATE
{
	game_state_main_menu, game_state_console
};

struct GAME_HANDLER
{
	ENTITY *entity_batch;
	u32 entity_batch_count;
	u32 entity_used;
	u32 entity_id;
    
	u32 vtx_id;
	u32 ibo_id;
    
	TILEMAP tilemap;
    
	ENTITY *player;
	ENTITY *monster;
	ENTITY *current_chosen_atlas;
	ENTITY *current_chosen_sprite;
	ENTITY *sprite_box;
    
	ENTITY *camera;

	CJ_CONSOLE console;
    
	GAME_STATE state = game_state_main_menu;
    
};



#endif
