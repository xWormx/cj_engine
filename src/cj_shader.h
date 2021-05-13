/* date = January 12th 2021 11:44 am */

#ifndef CJ_OPENGL_H
#define CJ_OPENGL_H

struct SHADER_PROGRAM
{
	u32 VS = {};

    	u32 FS = {};

    	u32 GS = {};

    	u32 shaderProgram = {};

	char *shaderBuffer;
	
};

void CreateShader(SHADER_PROGRAM *s, char *inFilePath, i32 shader_type);
void LinkShaderProgram(SHADER_PROGRAM *s);
void UseShaderProgram(SHADER_PROGRAM *s);
void Destroy(SHADER_PROGRAM *s);
void CheckShaderCompileErrors(SHADER_PROGRAM *s, unsigned int shader);
void CheckShaderLinkErrors(SHADER_PROGRAM *s);
void LoadShaderFile(SHADER_PROGRAM *s, char *inFilePath);
void GLPrintErrorOutputDBGstr();

void GLClearErrors()
{
    while(glGetError() != GL_NO_ERROR);
}

void GLPrintErrorOutputDBGstr()
{
    
    GLenum err = {};
    u8 err_info[1024] = {};
    u8 err_str[1024] = {};
    
    while(err = glGetError())
    {
        
        switch(err)
        {
            case GL_INVALID_ENUM:                  strcpy((char*)err_str, "INVALID_ENUM"); break;
            case GL_INVALID_VALUE:                 strcpy((char*)err_str, "INVALID_VALUE"); break;
            case GL_INVALID_OPERATION:             strcpy((char*)err_str, "INVALID_OPERATION"); break;
            case GL_STACK_OVERFLOW:                strcpy((char*)err_str, "STACK_OVERFLOW"); break;
            case GL_STACK_UNDERFLOW:               strcpy((char*)err_str, "STACK_UNDERFLOW"); break;
            case GL_OUT_OF_MEMORY:                 strcpy((char*)err_str, "OUT_OF_MEMORY"); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: strcpy((char*)err_str, "IVALID_FRAMEBUFFER_OPERATION"); break;
        }
        
        sprintf((char*)err_info, "(%s, %d): (%d) %s", __FILE__, __LINE__, err, err_str);
        OutputDebugString((char*)err_info);
    }
}

void CreateShader(SHADER_PROGRAM *s, char *inFilePath, i32 shader_type)
{
    if(shader_type == GL_VERTEX_SHADER)
    {
        
        LoadShaderFile(s, inFilePath);
        
        s->VS = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(s->VS, 1, &s->shaderBuffer, 0);
        glCompileShader(s->VS);
        
        CheckShaderCompileErrors(s, s->VS);
        free(s->shaderBuffer);
        
    }
    else if (shader_type == GL_FRAGMENT_SHADER)
    {
        
        LoadShaderFile(s, inFilePath);
        
        s->FS = glCreateShader(GL_FRAGMENT_SHADER);
        
        glShaderSource(s->FS, 1, &s->shaderBuffer, 0);
        glCompileShader(s->FS);
        
        CheckShaderCompileErrors(s, s->FS);
        free(s->shaderBuffer);
        
    }
}

void LinkShaderProgram(SHADER_PROGRAM *s)
{
    s->shaderProgram = glCreateProgram();
    
    GLClearErrors();
    glAttachShader(s->shaderProgram, s->VS);
    GLPrintErrorOutputDBGstr();
    
    GLClearErrors();
    glAttachShader(s->shaderProgram, s->FS);
    GLPrintErrorOutputDBGstr();
    
    GLClearErrors();
    // NOTE: Not using right now glAttachShader(s->shaderProgram, s->GS);
    GLPrintErrorOutputDBGstr();
    
    glLinkProgram(s->shaderProgram);
    
    CheckShaderLinkErrors(s);
    
}

void UseShaderProgram(SHADER_PROGRAM *s)
{
    glUseProgram(s->shaderProgram);
}

void destroy(SHADER_PROGRAM *s)
{
    glDeleteShader(s->VS);
    glDeleteShader(s->FS);
    glDeleteShader(s->GS);
}

void CheckShaderCompileErrors(SHADER_PROGRAM *s, unsigned int shader)
{
    
    int compilationSuccess; 
    char infoLog[1024];
    char outPutString[1024] = {};
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
    if(!compilationSuccess)
    {
        glGetShaderInfoLog(shader, 1024, 0, infoLog);
        if(shader == s->VS)
        {
            printf("!!ERROR-SHADER-VERTEX-COMPILATION_FAILED\n%s", infoLog);
            
            sprintf(outPutString, "!!ERROR-SHADER-VERTEX-COMPILATION_FAILED\n%s", infoLog);
            OutputDebugString(outPutString);
        }
        else if(shader == s->FS)
        {
            printf("!!ERROR-SHADER-FRAGMENT-COMPILATION_FAILED\n%s", infoLog);
            sprintf(outPutString, "!!ERROR-SHADER-FRAGMENT-COMPILATION_FAILED\n%s", infoLog);
            OutputDebugString(outPutString);
        }
        else if(shader == s->GS)
        {
            
            printf("!!ERROR-SHADER-GEOMETRY-COMPILATION_FAILED\n%s", infoLog);
            sprintf(outPutString, "!!ERROR-SHADER-GEOMETRY-COMPILATION_FAILED\n%s", infoLog);
            OutputDebugString(outPutString);
        }
    }
}

void CheckShaderLinkErrors(SHADER_PROGRAM *s)
{
    int linkSuccess = 0;
    char infoLog[1024] = {};
    char outPutString[1024] = {};
    
    glGetProgramiv(s->shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if(!linkSuccess)
    {
        glGetProgramInfoLog(s->shaderProgram, 1024, 0, infoLog);
        printf("ERROR-SHADER-PROGRAM-LINK_FAILED\n%s", infoLog);
        sprintf(outPutString, "!!ERROR-SHADER-PROGRAM-LINK-COMPILATION_FAILED\n%s", infoLog);
        OutputDebugString(outPutString);
    }
}

void LoadShaderFile(SHADER_PROGRAM *s, char *inFilePath)
{
    FILE *file = fopen(inFilePath, "r"); 
    
    if(!file)
    {
        printf("Can't open: %s\n", inFilePath);
    }
    
    int nCharsInFile = 1024; 
    char fileBuffer[1024] = {};
    s->shaderBuffer = (char*)malloc(1024);
    strcpy(s->shaderBuffer, "");
    
    while(fgets(fileBuffer, nCharsInFile, file) != 0)
    {
        strcat(s->shaderBuffer, fileBuffer);
    }
    
    printf("length of shader(nr of characters): %d\n", (int)strlen(s->shaderBuffer));
    
    fclose(file);
    
}



#endif //CJ_OPENGL_H
