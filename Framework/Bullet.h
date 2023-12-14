#pragma once

#ifndef BULLET_H
#define BULLET_H

#include "BaseObjects.h"
#include "CommonFunctions.h"


class Bullet : public BaseObjects 
{
public: 
	Bullet();
	~Bullet(); 

	enum BulletDir
	{
		RIGHT_DIR = 20,
		LEFT_DIR = 21,
		UP_DIR = 22,
		UP_RIGHT_DIR = 23,
		UP_LEFT_DIR = 24,
		DOWN_DIR = 25,
		DOWN_RIGHT_DIR = 26,
		DOWN_LEFT_DIR = 27,
	};

	enum BulletType
	{
		SPHERE_BULLET = 50,
		WATER_SHAPE_BULLET = 51,
	};

	void setX(const int& xVal) { xValue = xVal; }
	void setY(const int& yVal) { yValue = yVal; }
	int getX() const { return xValue; }
	int getY() const { return yValue;}

	void setIsMove(const bool& is_move) { isMove = is_move; }
	bool getIsMove() const { return isMove; }

	void setBulletDir(const unsigned int& BulletDir) { bulletDir = BulletDir; }
	int getBulletDir() const { return bulletDir; }

	void setBulletType(const unsigned int& BulletType) { bulletType = BulletType; }
	int getBulletType() const { return bulletType; }

	void HandleMove(const int& xBorder, const int& yBorder); 
	bool LoadBulletImg(SDL_Renderer* des); 

private:
	int xValue;
	int yValue; 
	bool isMove; 
	unsigned int bulletDir;
	unsigned int bulletType; 
};

#endif