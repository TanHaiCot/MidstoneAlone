#include "Bullet.h"

Bullet::Bullet()
{
	xValue = 0; 
	yValue = 0; 
	isMove = false; 
	bulletType = SPHERE_BULLET; 
}

Bullet::~Bullet() 
{
}


void Bullet::HandleMove(const int& xBorder, const int& yBorder)
{
	if (bulletDir == RIGHT_DIR)
	{
		rect.x += xValue;
		if (rect.x > xBorder)
		{
			isMove = false;
		}
	}
	else if (bulletDir == LEFT_DIR)
	{
		rect.x -= xValue;
		if (rect.x < 0)
		{
			isMove = false;
		}
	}
	else if (bulletDir == UP_DIR)
	{
		rect.y -= yValue;
		if (rect.y < 0)
		{
			isMove = false; 
		}
	}
	else if (bulletDir == UP_RIGHT_DIR)
	{
		rect.x += xValue; 
		if (rect.x > xBorder)
		{
			isMove = false; 
		}
		rect.y -= yValue; 
		if (rect.y < 0)
		{
			isMove == false; 
		}
	}
	else if (bulletDir == UP_LEFT_DIR)
	{
		rect.x -= xValue;
		if (rect.x < 0)
		{
			isMove = false;
		}
		rect.y -= yValue;
		if (rect.y < 0)
		{
			isMove == false;
		}
	}
}

bool Bullet::LoadBulletImg(SDL_Renderer* des)
{
	bool ret = false; 
	if (bulletType == WATER_SHAPE_BULLET)
	{
		ret = loadImage("textures/02.png", des);
	}
	else
	{
		ret = loadImage("textures/pellett.png", des);
	}

	return ret; 
}