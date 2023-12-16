#include "Map.h"

void GameMap::loadMap(const char* name)
{
	FILE* f = nullptr;
	fopen_s(&f, name, "rb");
	if (f == nullptr)
	{
		return;
	}

	gameMap.max_x = 0;
	gameMap.max_y = 0;


	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for (int j = 0; j < MAX_MAP_X; j++)
		{
			fscanf_s(f, "%d", &gameMap.tile[i][j]);
			int value = gameMap.tile[i][j];
			if (value > 0)
			{
				if (j > gameMap.max_x)
				{
					gameMap.max_x = j;
				}
				if (i > gameMap.max_y)
				{
					gameMap.max_y = i;
				}
			}
		}
	}
	gameMap.max_x = (gameMap.max_x + 1) * TILE_SIZE;
	gameMap.max_y = (gameMap.max_y + 1) * TILE_SIZE;

	gameMap.start_x = 0;
	gameMap.start_y = 0;

	gameMap.filename == name;
	fclose(f);
}


void GameMap::loadTiles(SDL_Renderer* screen)
{
	char file_img[40];
	FILE* f = nullptr;

	for (int i = 0; i < MAX_TILES; i++)
	{
		sprintf_s(file_img, "map/%d.png", i);

		fopen_s(&f, file_img, "rb");
		if (f == nullptr)
		{
			continue;
		}

		fclose(f);

		tileMap[i].loadImage(file_img, screen);
	}
}


void GameMap::drawMap(SDL_Renderer* screen)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	int map_x = 0;
	int map_y = 0;

	map_x = gameMap.start_x / TILE_SIZE;
	x1 = (gameMap.start_x % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	map_y = gameMap.start_y / TILE_SIZE;
	y1 = (gameMap.start_y % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

	for (int i = y1; i < y2; i += TILE_SIZE)
	{
		map_x = gameMap.start_x / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE)
		{
			int value = gameMap.tile[map_y][map_x];
			if (value > 0)
			{
				tileMap[value].SetRect(j, i);
				tileMap[value].Render(screen);
			}
			map_x++;
		}
		map_y++;
	}
}