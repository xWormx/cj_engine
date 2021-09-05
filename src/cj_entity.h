
enum GEOMETRY_TYPE
{
	GEOMETRY_TYPE_AABB, GEOMETRY_TYPE_CIRCLE
};

enum COLLISION_TYPE
{
	collision_type_ground, collision_type_structure, collision_type_creature, collision_type_edge
};

struct ENTITY
{
	V2f pos;
	V2f size;
	float rotation;
	V2f vertex_position[4];
	V2f vel;
	V2f texcoord_pos;
	V2f texcoord_size;
	V4f col;
	u32 quad_id;
	u32 id;
	u32 ibo_id;
	u32 geometric_type;
	u32 collision_type;

};

V2f GetEntityValue_VertexPosition(ENTITY *entity, u32 vertex);
void SetEntityValue_VertexPosition(ENTITY *entity);

void ClearEntity(ENTITY *e);

bool ClampEntityToScreen_XY(ENTITY *e);
bool ClampEntityToScreen_X(ENTITY *e);
bool ClampEntityToScreen_Y(ENTITY *e);

V2f GetEntityValue_VertexPosition(ENTITY *entity, u32 vertex)
{
	return entity->vertex_position[vertex];
}

void SetEntityValue_VertexPosition(ENTITY *entity)
{
	entity->vertex_position[0] = v2f(entity->pos.x, entity->pos.y);	
	entity->vertex_position[1] = v2f(entity->pos.x, entity->pos.y + entity->size.y);	
	entity->vertex_position[2] = v2f(entity->pos.x + entity->size.x, entity->pos.y + entity->size.y);	
	entity->vertex_position[3] = v2f(entity->pos.x + entity->size.x, entity->pos.y);	
}

void ClearEntity(ENTITY *e, u32 num_entities)
{
	for(u32 i = 0; i < num_entities; i++)
	{
		e[i].vel 		= v2f(0.f, 0.f);
		e[i].pos		= v2f(0.f, 0.f);
		e[i].size		= v2f(0.f, 0.f);
		e[i].texcoord_pos	= v2f(0.f, 0.f);
		e[i].texcoord_size	= v2f(0.f, 0.f);
		e[i].col		= v4f(0.f, 0.f, 0.f, 0.f);
	}
}

bool ClampEntityToScreen_XY(ENTITY *e)
{

	bool did_clamp = false;

	V2f pos = e->pos;
	V2f size = e->size;


	if(pos.y <= 0.0f)
	{
		e->pos.y = 0.0f;
		did_clamp = true;
	}
	if(pos.y + size.y >= 2.0f)
	{
		e->pos.y = 2.0f - size.y;
		did_clamp = true;
	}
	if(pos.x <= 0.0f)
	{
		e->pos.x = 0.0f;
		did_clamp = true;
	}
	if(pos.x + size.x >= 2.0f)
	{
		e->pos.x = 2.0f - size.x;
		did_clamp = true;
	}

	return did_clamp;
	
}

bool ClampEntityToScreen_Y(ENTITY *e)
{
	bool did_clamp = false;

	V2f pos = e->pos;
	V2f size = e->size;

	if(pos.y <= 0.0f)
	{
		e->pos.y = 0.0f;
		did_clamp = true;
	}
	if(pos.y + size.y >= 2.0f)
	{
		e->pos.y = 2.0f - size.y;
		did_clamp = true;
	}

	return did_clamp;
}

bool ClampEntityToScreen_X(ENTITY *e)
{
	bool did_clamp = false;

	V2f pos = e->pos;
	V2f size = e->size;

	if(pos.x <= 0.0f)
	{
		e->pos.x = 0.0f;
		did_clamp = true;
	}
	if(pos.x + size.x >= 2.0f)
	{
		e->pos.x = 2.0f - size.x;
		did_clamp = true;
	}

	return did_clamp;
}

