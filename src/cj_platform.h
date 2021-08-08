#ifndef CJ_PLATFORM_H
#define CJ_PLATFORM_H



struct CJ_PLATFORM
{
	GLFWwindow *window;
	i32 win_w;
	i32 win_h;
	V2d cursor_pos;
	V2i cursor_pos_int;
	double frame_endtime;
	double frame_starttime;
	double time_per_frame;

};


CJ_PLATFORM CreatePlatform(u32 w, u32 h, char *title, u32 fullscreen);
bool KeyPressed(CJ_PLATFORM *platform, i32 key);
void key_callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods);
void char_callback(GLFWwindow *window, u32 code_point);
void GetCursorPos(CJ_PLATFORM *platform);
void GetWindowSize(CJ_PLATFORM *platform);

void UpdatePlatform(CJ_PLATFORM *platform);

void SwapBuffers(CJ_PLATFORM platform);

CJ_PLATFORM CreatePlatform(u32 w, u32 h, char *title, u32 fullscreen)
{
	CJ_PLATFORM platform = {};

  	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if(!glfwInit())
	{
		printf("failed to init glfw\n");
	}

	platform.window = glfwCreateWindow(w, h, "HEJ", 0, 0);
	if(!platform.window)
	{
		printf("failed to create window\n");
	}
	else
	{
		glfwMakeContextCurrent(platform.window);
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		{
			printf("failed to load glad\n");
		}
	}

	glfwSetKeyCallback(platform.window, key_callback);
	glfwSetCharCallback(platform.window, char_callback);
	glfwSwapInterval(1);


	return platform;

}

double GetTime()
{
	return glfwGetTime();
}

bool KeyPressed(CJ_PLATFORM *platform, i32 key)
{
	if(glfwGetKey(platform->window, key) == GLFW_PRESS) return true;
	else return false;
}

bool MousePressed(CJ_PLATFORM *platform, i32 key)
{
	if(glfwGetMouseButton(platform->window, key) == GLFW_PRESS) return true;
	else return false;
}

bool input_key_once[512] = {};
bool input_key_once_blocker[512] = {};
bool input_mouse_once[32] = {};
bool input_mouse_once_blocker[512] = {};

bool KeyPressedOnce(u32 key)
{
	return input_key_once[key];

}
bool MousePressedOnce(u32 key)
{
	return input_mouse_once[key];

}

void ProcessInput(CJ_PLATFORM *platform)
{
	for(u32 i = 0; i < ArrayCount(input_key_once); i++)
	{
		if(KeyPressed(platform, i))
		{
			if(!input_key_once_blocker[i])
			{
				input_key_once[i] = true;
				input_key_once_blocker[i] = true;
			}
			else
			{
				input_key_once[i] = false;
			}
		}
		else
		{
			input_key_once_blocker[i] = false;
		}
	}

	for(u32 i = 0; i < ArrayCount(input_mouse_once); i++)
	{
		if(MousePressed(platform, i))
		{
			if(!input_mouse_once_blocker[i])
			{
				input_mouse_once[i] = true;
				input_mouse_once_blocker[i] = true;
			}
			else
			{
				input_mouse_once[i] = false;
			}
		}
		else
		{
			input_mouse_once_blocker[i] = false;
		}
	}
}

void key_callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods)
{

}


char textInput[1] = {};
bool charPressed = false;
void char_callback(GLFWwindow *window, u32 code_point)
{
	if(!charPressed)
	{
		*textInput = code_point;
		printf("%s\n", textInput);
	}

}


void GetCursorPos(CJ_PLATFORM *platform)
{
	double x = 0.0f;
	double y = 0.0f;
	glfwGetCursorPos(platform->window, &x, &y);
	platform->cursor_pos.x = 2.0f * x / (platform->win_w);
	platform->cursor_pos.y = 2.0f - ((2.0f * y) / (platform->win_h));
}

void GetCursorPos_INT(CJ_PLATFORM *platform)
{
	double x = 0.0f;
	double y = 0.0f;
	glfwGetCursorPos(platform->window, &x, &y);
	platform->cursor_pos_int.x = x;
	platform->cursor_pos_int.y = platform->win_h - y;
}

void GetWindowSize(CJ_PLATFORM *platform)
{
	glfwGetWindowSize(platform->window, &platform->win_w, &platform->win_h);
}

void UpdatePlatform(CJ_PLATFORM *platform)
{
	
	platform->frame_endtime = GetTime();
	platform->time_per_frame = platform->frame_endtime - platform->frame_starttime;
	platform->frame_starttime = GetTime();

	GetWindowSize(platform);
	GetCursorPos(platform);
	GetCursorPos_INT(platform);
	ProcessInput(platform);
	glfwPollEvents();
}

void SwapBuffers(CJ_PLATFORM platform)
{
	glfwSwapBuffers(platform.window);
}



#endif
