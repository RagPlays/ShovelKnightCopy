#pragma once
//ML Detection Extension
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

// SDL and OpenGL Includes
#pragma warning(disable : 26812)
#pragma warning(disable : 4820)
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h> 
#include <SDL_image.h>

#pragma warning(default : 26812)

// Always Handy
#include <iostream>

// Saving / Loading Files
#include <sstream>
#include <fstream>

// hashtable
#include <unordered_map>

// From FrameWork
#include "structs.h"
#include "utils.h"
#include "Texture.h"
#include "Vector2f.h"
#include "SoundStream.h"
#include "SoundEffect.h"