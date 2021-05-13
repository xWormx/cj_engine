@echo off

SET CJ_LIB=C:\CJ_LIB
SET GLAD=C:\GLAD\src
SET GLFW_INCLUDE="..\GLFW\glfw-3.3.bin.WIN64\include\GLFW"
SET GLFW_LIB="..\GLFW\glfw-3.3.bin.WIN64\lib-vc2019"
SET STB=C:\stb

pushd ..\build

cl /F 800000000 /EHsc /MD /wd4312 -Zi -nologo /I %STB% /I %CJ_LIB% /I %GLFW_INCLUDE% /I %GLAD% /wd4005 ..\src\engine.cpp  /link user32.lib gdi32.lib opengl32.lib shell32.lib /LIBPATH:%CJ_LIB% cj_vector.lib /LIBPATH:%GLFW_LIB% glfw3.lib glfw3dll.lib

popd


del *.un *.un~ *.cpp~ *.swp *.bat~ *.h~
