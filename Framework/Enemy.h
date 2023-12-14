#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include "CommonFunctions.h"
#include "BaseObjects.h"
#include "Bullet.h"

#define ENEMY_MAX_FALL_SPEED 10
#define ENEMY_GRAVITY_SPEED 0.8
#define ENEMY_FRAME_NUMBER 8
#define ENEMY_SPEED 3 
class Enemy : public BaseObjects
{
public:
	Enemy();
	~Enemy();

	enum MoveType
	{
		STATIC_ENEMY = 0,
		MOVE_IN_AREA = 1,

	};
	void setXval(const float& x_val) { valX = x_val; }
	void setYval(const float& y_val) { valY = y_val; }

	void setXpos(const float& x_pos) { posX = x_pos; }
	void setYpos(const float& y_pos) { posY = y_pos; }
	float getXpos() const { return posX; }
	float getYpos() const { return posY; }
	void SetMapXY(const int& map_x, const int& map_y) { mapX = map_x; mapY = map_y; }

	void SetClip();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des); 
	int GetWidthFrame() const { return WidthFrame;}
	int GetHeightFrame() const { return HeightFrame; }
	void Action(Map& map); 
	void CheckToMap(Map& map);
	void InitEnemy();
	void SetMoveType(const int& Movetype) { moveType = Movetype; }
	void SetAnimationPos(const int& posA, const int& posB) { animationA = posA; animationB = posB; }
	void SetInputLeft(const int& ipLeft) { input.left = ipLeft; }
	void ImpMoveType(SDL_Renderer* screen); 
	SDL_Rect GetRectFrame();
	
	std::vector<Bullet*> GetBulletList() const { return BulletList; }
	void SetBulletList(const std::vector<Bullet*>& bulletList) { BulletList = bulletList; }

	void InitBullet(Bullet* bullet, SDL_Renderer* screen); 
	void CreateBullet(SDL_Renderer* screen, const int& xLimit, const int& yLimit); 
	void RemoveBullet(const int& index);
	
private:
	int mapX;
	int mapY;
	float valX;
	float valY;
	float posX;
	float posY;
	bool OnGround;
	int comebackTime;
	SDL_Rect frameClip[ENEMY_FRAME_NUMBER];
	int WidthFrame;
	int HeightFrame;
	int Frame; 

	int moveType; 
	int animationA; 
	int animationB; 
	Input input; 

	std::vector<Bullet*> BulletList;
};

#endif