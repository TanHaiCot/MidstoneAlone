#pragma once

#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCITON_H_

#include <Windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <SDL_ttf.h>

static SDL_Window* window = nullptr; 
static SDL_Renderer* screen = nullptr;
static SDL_Event event;

static Mix_Chunk* bulletSound; 
static Mix_Chunk* jumpSound; 
static Mix_Music* backgroundMusic; 

//Screen
const int FRAME_PER_SECOND = 25;
const int SCREEN_WIDTH = 1280; 
const int SCREEN_HEIGHT = 640; 
const int SCREEN_BPP = 32; 

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175; 
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff; 


#define BLANK_TILE 0
#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

#define COIN 4  // the number of the png
typedef struct Input
{
	int left; 
	int right;
	int up;
	int down;
	int jump;
};

typedef struct Map
{
	int start_x;
	int start_y;

	int max_x;
	int max_y;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* filename;
};


namespace SDLCommonFunctions
{
	bool CheckCollision(const SDL_Rect& obj1, const SDL_Rect& obj2); 
}

#endif