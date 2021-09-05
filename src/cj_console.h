#ifndef CJ_CONSOLE_H
#define CJ_CONSOLE_H

enum CJ_CONSOLE_MODE
{
		cj_console_mode_off, cj_console_mode_on
};

struct CJ_CONSOLE
{
	ENTITY *bkg_window;
	V2f	end_pos;
	V2f	start_pos;
	V4f color;
	CJ_CONSOLE_MODE mode;

	char str_recieved_from_global_input;
	u8 	*str_buffer_to_use;
	u8 	*str_base;
	u8	*str_clear_buffer;
	u32	buffer_size;
	i32	char_index;
	u32 max_console_chars_to_render;

	bool cleared_before_use;
	bool executed;


	float	alpha_mul;
};

void HandleConsole(CJ_CONSOLE *console);
void BeginConsole(CJ_CONSOLE *console, u32 n_chars_to_allocate);
void UpdateConsole(CJ_CONSOLE *console);
bool ConsoleBufferIsEmpty(CJ_CONSOLE *console);
void ClearConsoleBeforeUse(CJ_CONSOLE *console);
void ClearConsoleBuffer(CJ_CONSOLE *console);
void EndConsole(CJ_CONSOLE *console);

void HandleConsole(CJ_CONSOLE *console)
{
	BeginConsole(console, console->max_console_chars_to_render);
	UpdateConsole(console);
}


void BeginConsole(CJ_CONSOLE *console, u32 n_chars_to_allocate)
{
	if(!console->str_base)
	{
		console->buffer_size						= n_chars_to_allocate * sizeof(u8);
		console->str_buffer_to_use 					= (u8*)VirtualAlloc(0, n_chars_to_allocate, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		console->str_base 							= (u8*)VirtualAlloc(0, n_chars_to_allocate, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		console->str_clear_buffer					= (u8*)VirtualAlloc(0, n_chars_to_allocate, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		console->char_index 						= 0;
		console->mode								= cj_console_mode_off;
		console->end_pos							= v2f(0.0f, 1.0f);
		console->start_pos							= v2f(0.0f, 2.0f);
		console->color								= COLOR_GREEN;
		console->executed							= false;
	}

}


void UpdateConsole(CJ_CONSOLE *console)
{

		if(console->mode == cj_console_mode_on)
		{
			console->executed = false;

			if(console->bkg_window->pos.y >= console->end_pos.y)
			{
					console->bkg_window->pos.y -= 0.3f;
			}
			else
			{
				if(!console->cleared_before_use)
				{
					ClearConsoleBeforeUse(console);
				}
				else
				{
					console->str_recieved_from_global_input = *global_text_input;
					if(console->str_recieved_from_global_input != '\0' &&
					   console->str_recieved_from_global_input >= 0    &&	// make sure one doesn't write 'ö' which will draw a blank space....
					   console->str_recieved_from_global_input <= 127  &&
					   console->char_index < console->max_console_chars_to_render)
					{
						assert(console->char_index <= console->buffer_size);
						printf("index: %d\n", console->char_index);
						printf("size: %d\n", console->buffer_size);
						strcat((char*)console->str_base, &console->str_recieved_from_global_input);
						console->str_recieved_from_global_input = '\0';
						console->char_index++;
					}
					else if(KeyPressedOnce(GLFW_KEY_ENTER))
					{
						strcpy((char*)console->str_buffer_to_use, (char*)console->str_base);
						ClearConsoleBuffer(console);
						console->str_recieved_from_global_input = '\0';
						printf("console buffer: %s\n", console->str_buffer_to_use);
						console->executed = true;
					}
					else if(KeyPressedOnce(GLFW_KEY_BACKSPACE))
					{
						console->char_index--;
						if(console->char_index < 0)
						{
							console->char_index = 0;
						}
						else
						{
							console->str_base[console->char_index] = '\0';
							console->str_recieved_from_global_input = '\0';
						}
					}
				}
			}

		}
		else
		{
				if(console->bkg_window->pos.y <= console->start_pos.y)
				{
						console->bkg_window->pos.y += 0.3f;
				}
				ClearConsoleBuffer(console);
				console->cleared_before_use = false;
		}
}

bool ConsoleBufferIsEmpty(CJ_CONSOLE *console)
{
	bool result;
	if(*console->str_base == '\0')
	{
			return true;
	}
	else
	{
			return false;
	}
	

}

void ClearConsoleBeforeUse(CJ_CONSOLE *console)
{
	if(!console->cleared_before_use)
	{
			ClearConsoleBuffer(console);
			console->cleared_before_use 				= true;
			console->str_recieved_from_global_input 	= '\0';
	}
}

void ClearConsoleBuffer(CJ_CONSOLE *console)
{
	if(!ConsoleBufferIsEmpty(console))
	{
		char clearBuffer[2056] = {};
		strcpy((char*)console->str_base, (char*)console->str_clear_buffer);
		console->char_index = 0;
	}
}



void EndConsole(CJ_CONSOLE *console)
{
	VirtualFree(console->str_buffer_to_use, 0, MEM_RELEASE);
	VirtualFree(console->str_base, 0, MEM_RELEASE);
	VirtualFree(console->str_clear_buffer, 0, MEM_RELEASE);
}
#endif
