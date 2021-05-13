@echo off

SET SDL_INCLUDE=C:\SDL\SDL2-2.0.12\include
SET SDL_IMAGE_INCLUDE=C:\SDL\SDL2_image-2.0.5\include
SET SDL_TTF_INCLUDE=C:\SDL\SDL2_ttf-2.0.15\include

cl -nologo -Zi /c -EHsc cj_vector.cpp
lib -nologo cj_vector.obj

cl -nologo -Zi /c -EHsc cj_network.cpp
lib -nologo cj_network.obj


cl -Zi -nologo -EHsc /c /I %SDL_INCLUDE% /I %SDL_IMAGE_INCLUDE% /I %SDL_TTF_INCLUDE% cj_lib.cpp
lib -nologo cj_lib.obj
