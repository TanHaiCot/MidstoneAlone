#pragma once

#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunctions.h"
#include "BaseObjects.h"

#define MAX_TILES 20
class TileMap : public BaseObjects
{
public:
	TileMap() { ; }
	~TileMap() { ; }
};


class GameMap 
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void loadMap(const char* name);
	void loadTiles(SDL_Renderer* screen);
	void drawMap(SDL_Renderer* screen); 
	Map getMap() const { return gameMap; };
	void setMap(Map& mapData) { gameMap = mapData; }

private:
	Map gameMap;
	TileMap tileMap[MAX_TILES]; 
};

#endif 