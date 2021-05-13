#ifndef CJ_PLATFORM_H
#define CJ_PLATFORM_H



struct CJ_PLATFORM
{
	GLFWwindow *window;
	i32 win_w;
	i32 win_h;
	V2d cursor_pos;

};


CJ_PLATFORM CreatePlatform(u32 w, u32 h, char *title, u32 fullscreen);
bool GetKey(CJ_PLATFORM *platform, i32 key);
void GetCursorPos(CJ_PLATFORM *platform);
void GetWindowSize(CJ_PLATFORM *platform);

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

	platform.window = glfwCreateWindow(640, 480, "HEJ", 0, 0);
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

	glfwSwapInterval(1);


	return platform;

}


bool GetKey(CJ_PLATFORM *platform, i32 key)
{
	if(glfwGetKey(platform->window, key) == GLFW_PRESS) return true;
	else return false;

}

void GetCursorPos(CJ_PLATFORM *platform)
{
	double x = 0.0f;
	double y = 0.0f;
	glfwGetCursorPos(platform->window, &x, &y);
	platform->cursor_pos.x = 2.0f * x / (platform->win_w);
	platform->cursor_pos.y = 2.0f - ((2.0f * y) / (platform->win_h));
}

void GetWindowSize(CJ_PLATFORM *platform)
{
	glfwGetWindowSize(platform->window, &platform->win_w, &platform->win_h);
}

void SwapBuffers(CJ_PLATFORM platform)
{
	glfwSwapBuffers(platform.window);
}



#endif
