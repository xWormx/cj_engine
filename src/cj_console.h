#ifndef CJ_CONSOLE_H
#define CJ_CONSOLE_H

struct CJ_CONSOLE
{
	u8 	*str_base;
	u8	*str_clear_buffer;
	u32	buffer_size;
	i32 	char_index;

	float	alpha_mul;
};


void BeginConsole(CJ_CONSOLE *console, u32 n_chars_to_allocate)
{
	if(!console->str_base)
	{
		console->buffer_size		= n_chars_to_allocate * sizeof(u8);
		console->str_base 		= (u8*)VirtualAlloc(0, n_chars_to_allocate, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		console->str_clear_buffer	= (u8*)VirtualAlloc(0, n_chars_to_allocate, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		console->char_index 		= 0;
	}

}

void UpdateConsole(CJ_CONSOLE *console)
{

	if(*textInput != '\0' &&
	   *textInput >= 0    &&	// make sure one doesn't write 'ö' which will draw a blank space....
	   *textInput <= 127  &&
	   (console->char_index) <= console->buffer_size)   	 
	{

		printf("index: %d\n", console->char_index);
		printf("size: %d\n", console->buffer_size);
		strcat((char*)console->str_base, textInput);
		*textInput = '\0';
		console->char_index++;
	}
	else if(KeyPressedOnce(GLFW_KEY_ENTER))
	{
		char clearBuffer[2056] = {};
		strcpy((char*)console->str_base, (char*)console->str_clear_buffer);
		console->char_index = 0;
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
		}
	}
}



void EndConsole(CJ_CONSOLE *console)
{
	VirtualFree(console->str_base, 0, MEM_RELEASE);
	VirtualFree(console->str_clear_buffer, 0, MEM_RELEASE);
}
#endif
