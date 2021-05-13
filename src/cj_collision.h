#ifndef _CJ_COLLISION_H
#define _CJ_COLLISION_H


struct COLLISION_INFO
{
	float f_min_penetration;
	float f_direction;
	bool collided;
	V2f mtv;
};

bool Collision_AABB(ENTITY *e1, ENTITY *e2);

COLLISION_INFO Collision_SAT_ENTITY_ENTITY(ENTITY *e1, ENTITY *e2);

bool Collision_AABB(ENTITY *e1, ENTITY *e2)
{
	bool collided = false;
	float e1_L = e1->pos.x;
	float e1_R = e1->pos.x + e1->size.x;
	float e1_T = e1->pos.y + e1->size.y;
	float e1_B = e1->pos.y;

	float e2_L = e2->pos.x;
	float e2_R = e2->pos.x + e2->size.x;
	float e2_T = e2->pos.y + e2->size.y;
	float e2_B = e2->pos.y;

	if(e1_R > e2_L && 
	   e1_L < e2_R &&
	   e1_T > e2_B &&
	   e1_B < e2_T)
	{
		collided = true;
	}

	return collided;
}

COLLISION_INFO Collision_SAT_ENTITY_ENTITY(ENTITY *entity_1, ENTITY *entity_2)
{

	COLLISION_INFO result = {};

	SetEntityValue_VertexPosition(entity_1);
	SetEntityValue_VertexPosition(entity_2);

	V2f e1_points[4] 	= {};
	V2f e1_unit_v[4] 	= {};
	V2f e1_sides[4] 	= {};
	V2f e1_normal[4] 	= {};
	float dot_e1[4]		= {};

	V2f e2_points[4]	= {};
	V2f e2_unit_v[4] 	= {};
	V2f e2_sides[4] 	= {};
	V2f e2_normal[4] 	= {};
	float dot_e2[4]		= {};

	float overlap 			= 10000000.0f;
	bool collided			= false;
	V2f min_translation_axis	= v2f(0.0f, 0.0f);
	float tempOverlap = 0.0f;
	float f_direction = 0.0f;

	// Get all vertex points.
	for(i32 i = 0; i < 4; i++)
	{
		e1_points[i] = GetEntityValue_VertexPosition(entity_1, i);
		e2_points[i] = GetEntityValue_VertexPosition(entity_2, i);
	}


	// Get all normals to both entities and make them unit vectors.
	for(i32 i = 0; i < 4; i++)
	{
		if(i == 3)
		{
			e1_normal[i] = Unit_v2f(NormalTo_v2f(e1_points[i] - e1_points[i - 3]));
			e2_normal[i] = Unit_v2f(NormalTo_v2f(e2_points[i] - e2_points[i - 3]));
		}
		else
		{
			e1_normal[i] = Unit_v2f(NormalTo_v2f(e1_points[i] - e1_points[i + 1]));
			e2_normal[i] = Unit_v2f(NormalTo_v2f(e2_points[i] - e2_points[i + 1]));
		}

		//if(e1_normal[i].x < 0.0f) e1_normal[i].x *= -1.0f;
		//if(e1_normal[i].y < 0.0f) e1_normal[i].y *= -1.0f;
		//if(e2_normal[i].x < 0.0f) e2_normal[i].x *= -1.0f;
		//if(e2_normal[i].y < 0.0f) e2_normal[i].y *= -1.0f;
	}

	for(i32 j = 0; j < 4; j++)
	{
		for(i32 i = 0; i < 4; i++)
		{
			dot_e1[i] = Dot_v2f(e1_normal[j], e1_points[i]);
			dot_e2[i] = Dot_v2f(e1_normal[j], e2_points[i]);
		}

		float min_dot_e1 = Min_f(dot_e1, sizeof(dot_e1) / sizeof(float));
		float max_dot_e1 = Max_f(dot_e1, sizeof(dot_e1) / sizeof(float));
		float min_dot_e2 = Min_f(dot_e2, sizeof(dot_e2) / sizeof(float));
		float max_dot_e2 = Max_f(dot_e2, sizeof(dot_e2) / sizeof(float));

		tempOverlap = Overlap_f(min_dot_e1, max_dot_e1, min_dot_e2, max_dot_e2);
		f_direction = 0.0f;

		if(tempOverlap <= 0.0f)
		{
			overlap 			= 0.0f;
			collided			= false;

			result.f_min_penetration	= overlap;
			result.f_direction 		= f_direction;
			result.collided			= collided;

			return result;

		}
		else
		{
			if(tempOverlap < overlap)
			{
				//handle direction		
				if(min_dot_e1 < min_dot_e2)
				{
					f_direction = -1.0f;
				}
				else
				{
					f_direction = 1.0f;
				}

				overlap	= tempOverlap;
				result.f_min_penetration = overlap;
				result.f_direction = f_direction;
				min_translation_axis = e1_normal[j];
			}




		}
		


	}

	collided = true;
	float scalar = result.f_direction * result.f_min_penetration;
	result.mtv = Scale_v2f(scalar, min_translation_axis);
	result.collided = collided;

	return result;
}



#endif
