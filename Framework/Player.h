#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "CommonFunctions.h"
#include "BaseObjects.h"
#include <vector>
#include "Bullet.h"



#define GRAVITY_SPPED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 15
#define PLAYER_JUMP 18
//#define CONTROLLER_DEAD_ZONE 8000

class Player : public BaseObjects
{
public: 
	Player();
	~Player();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2,
	};

	bool loadImage(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bulletSound, Mix_Chunk* jumpSound);
	void HandleGameControllerInput(SDL_GameController* gameController, SDL_Renderer* screen, Mix_Chunk* bulletSound, Mix_Chunk* jumpSound);
	void setClip(); 

	void Play(Map& mapData);
	void CheckToMap(Map& mapData);
	void setMapXY(const int map_x, const int map_y) { mapX = map_x; mapY = map_y; }
	void mapMovement(Map& mapData); 
	void UpdatePlayerImage(SDL_Renderer* des);
	SDL_Rect GetRectFrame();

	void setBulletMag(std::vector<Bullet*> bulletMag)
	{
		BulletMag = bulletMag; 
	}
	std::vector<Bullet*> getBulletMag() const { return BulletMag; }
	void HandleBullet(SDL_Renderer* des);
	void RemoveBullet(const int& index); 
	void CoinIncrease();
	void SetReviveTime(const int& rvTime) { reviveTime = rvTime; }
	int GetCoinCount() const { return collectedCoin; }

private:
	std::vector<Bullet*> BulletMag; 

	int collectedCoin; 

	float x_value; 
	float y_value;

	float x_pos;   // x of the whole map: 0 -> 22k
	float y_pos;

	int widthFrame;
	int heightFrame; 

	SDL_Rect frameClip[8]; 
	Input inputType; 
	int frame;
	int movementStatus; 
	bool onGround; 

	int mapX;
	int mapY;  

	int reviveTime; 

	bool isUsingController; 
	float gameControllerDeadzone; 
};

#endif