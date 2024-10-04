#include "Enemy.h"
//#include <memory>
//#include <iostream>
//
//
//void* operator new(std::size_t numBytes) {
//	std::cout << "allocating " << numBytes << " bytes of memory\n";
//	return std::malloc(numBytes);
//}
//
//void operator delete(void* memoryLocation, std::size_t numBytes) {
//	std::cout << "freeing " << numBytes << " bytes of memory\n";
//	std::free(memoryLocation);
//}

Enemy::Enemy() 
{
    valX = 0;
    valY = 0;
    posX = 0;
    posY = 0;
	OnGround = false;
	comebackTime = 0;
	WidthFrame = 0;
	HeightFrame = 0;
	Frame = 0;

	animationA = 0;
	animationB = 0;
	input.left = 0; 
	moveType = STATIC_ENEMY; 
}


Enemy::~Enemy()
{
}


bool Enemy::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObjects::loadImage(path, screen); 
	if (ret == true)
	{
		WidthFrame = rect.w / ENEMY_FRAME_NUMBER; 
		HeightFrame = rect.h; 
	}
	return ret; 
}


SDL_Rect Enemy::GetRectFrame()
{
	SDL_Rect Rect;
	Rect.x = rect.x;
	Rect.y = rect.y;
	Rect.w = WidthFrame;
	Rect.h = HeightFrame;
	return Rect;
}


void Enemy::SetClip()
{
	if (WidthFrame > 0 && HeightFrame > 0)
	{
		frameClip[0].x = 0;
		frameClip[0].y = 0;
		frameClip[0].w = WidthFrame;
		frameClip[0].h = HeightFrame;

		frameClip[1].x = WidthFrame;
		frameClip[1].y = 0;
		frameClip[1].w = WidthFrame;
		frameClip[1].h = HeightFrame;

		frameClip[2].x = 2 * WidthFrame;
		frameClip[2].y = 0;
		frameClip[2].w = WidthFrame;
		frameClip[2].h = HeightFrame;

		frameClip[3].x = 3 * WidthFrame;
		frameClip[3].y = 0;
		frameClip[3].w = WidthFrame;
		frameClip[3].h = HeightFrame;

		frameClip[4].x = 4 * WidthFrame;
		frameClip[4].y = 0;
		frameClip[4].w = WidthFrame;
		frameClip[4].h = HeightFrame;

		frameClip[5].x = 5 * WidthFrame;
		frameClip[5].y = 0;
		frameClip[5].w = WidthFrame;
		frameClip[5].h = HeightFrame;

		frameClip[6].x = 6 * WidthFrame;
		frameClip[6].y = 0;
		frameClip[6].w = WidthFrame;
		frameClip[6].h = HeightFrame;

		frameClip[7].x = 7 * WidthFrame;
		frameClip[7].y = 0;
		frameClip[7].w = WidthFrame;
		frameClip[7].h = HeightFrame;
	}
}


void Enemy::Show(SDL_Renderer* des)
{
	if (comebackTime == 0)
	{
		rect.x = posX - mapX; 
		rect.y = posY - mapY; 
		Frame++; 
		if (Frame >= ENEMY_FRAME_NUMBER)
		{
			Frame = 0; 
		}

		SDL_Rect* currentClip = &frameClip[Frame]; 
		SDL_Rect rendQuad = { rect.x, rect.y, WidthFrame, HeightFrame };
		SDL_RenderCopy(des, object, currentClip, &rendQuad);
	}
}


void Enemy::Action(Map& map)
{
	if (comebackTime == 0)
	{
		valX = 0; 
		valY += ENEMY_GRAVITY_SPEED;
		if (valY >= ENEMY_MAX_FALL_SPEED)
		{
			valY = ENEMY_MAX_FALL_SPEED; 
		}

		if (input.left == 1)
		{
			valX -= ENEMY_SPEED; 
		}
		else if (input.right == 1)
		{
			valX += ENEMY_SPEED; 
		}

		CheckToMap(map); 
	}
	else if (comebackTime > 0)
	{
		comebackTime--; 
		if (comebackTime == 0)
		{
			InitEnemy();
		}
	}
}


void Enemy::InitEnemy()
{
	valX = 0;
	valY = 0;
	if (posX > 256)
	{
		posX -= 256;
		animationA -= 256;
		animationB -= 256;
	}
	else
	{
		posX = 0;
	}
	posY = 0;
	comebackTime = 0;
	input.left = 1;
}


//void Enemy::RemoveBullet(const int& index)
//{
//	int size = BulletList.size();
//	if (size > 0 && index < size)
//	{
//		Bullet* bullet = BulletList.at(index);
//		BulletList.erase(BulletList.begin() + index);
//		if (bullet)
//		{
//			delete bullet;
//			bullet = nullptr;
//		}
//	}
//}


void Enemy::CheckToMap(Map& map)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;


	//Check horizontal
	int heightMin = HeightFrame < TILE_SIZE ? HeightFrame : TILE_SIZE;

	x1 = (posX + valX) / TILE_SIZE;
	x2 = (posX + valX + WidthFrame - 1) / TILE_SIZE;

	y1 = (posY) / TILE_SIZE;
	y2 = (posY + heightMin - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (valX > 0) //the player are moving to right 
		{
			int val1 = map.tile[y1][x2];
			int val2 = map.tile[y2][x2];

			if ((val1 != BLANK_TILE && val1 != COIN) || (val2 != BLANK_TILE && val2 != COIN))
			{
				posX = x2 * TILE_SIZE;
				posX -= (WidthFrame + 1);
				valX = 0;
			}
		}
		else if (valX < 0)
		{
			int val1 = map.tile[y1][x1];
			int val2 = map.tile[y2][x1];

			if ((val1 != BLANK_TILE && val1 != COIN) || (val2 != BLANK_TILE && val2 != COIN))
			{
				posX = (x1 + 1) * TILE_SIZE;
				valX = 0;
			}
			
		}
	}


	//Check vertical 
	int widthMin = WidthFrame < TILE_SIZE ? WidthFrame : TILE_SIZE;
	x1 = posX / TILE_SIZE;
	x2 = (posX + widthMin) / TILE_SIZE;

	y1 = (posY + valY) / TILE_SIZE;
	y2 = (posY + valY + HeightFrame - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (valY > 0)
		{
			int val1 = map.tile[y2][x1];
			int val2 = map.tile[y2][x2];

			if ((val1 != BLANK_TILE && val1 != COIN) || (val2 != BLANK_TILE && val2 != COIN))
			{
				posY = y2 * TILE_SIZE;
				posY -= (HeightFrame + 1);
				valY = 0;
				OnGround = true;
			}
		}
		else if (valY < 0)
		{
			int val1 = map.tile[y1][x1];
			int val2 = map.tile[y1][x2];
			
			if ((val1 != BLANK_TILE && val1 != COIN) || (val2 != BLANK_TILE && val2 != COIN))
			{
				posY = (y1 + 1) * TILE_SIZE;
				valY = 0;
			}
		}
	}

	posX += valX;
	posY += valY;

	if (posX < 0)
	{
		posX = 0;
	}
	else if (posX + WidthFrame > map.max_x)
	{
		posX = map.max_x - WidthFrame - 1;
	}

	if (posY > map.max_y)
	{
		comebackTime = 60;
	}
}


void Enemy::ImpMoveType(SDL_Renderer* screen)
{
	if (moveType == STATIC_ENEMY)
	{
		;
	}
	else
	{
		if (OnGround == true)
		{
			if (posX > animationB)
			{
				input.left = 1;		
				input.right = 0; 
				LoadImg("textures/threat_left.png", screen); 
			}
			else if (posX < animationA)
			{
				input.left = 0;
				input.right = 1;
				LoadImg("textures/threat_right.png", screen);
			}
		}
		else
		{
			if (input.left == 1)
			{
				LoadImg("textures/threat_left.png", screen);
			}
		}
	}
}


void Enemy::InitBullet(Bullet* bullet, SDL_Renderer* screen)
{
	if (bullet != nullptr)
	{
		bullet->setBulletType(Bullet::WATER_SHAPE_BULLET);
		bool ret = bullet->LoadBulletImg(screen);

		if (ret)
		{
			bullet->setIsMove(true);
			bullet->setBulletDir(Bullet::LEFT_DIR);

			rect.x = posX - mapX;
			rect.y = posY - mapY;

			bullet->SetRect(rect.x + 10, rect.y + 10);
			bullet->setX(15);
			BulletList.push_back(bullet);
		}
	}
}


void Enemy::CreateBullet(SDL_Renderer* screen, const int& xLimit, const int& yLimit)
{
	for (int i = 0; i < BulletList.size(); i++)
	{
		Bullet* bullet = BulletList.at(i); 
		if (bullet != nullptr)
		{
			if (bullet->getIsMove())
			{
				int ShootingRange = rect.x + WidthFrame - bullet->GetRect().x;
				if (ShootingRange > 0 && ShootingRange < 300)
				{
					bullet->HandleMove(xLimit, yLimit);
					bullet->Render(screen);
				}
				else
				{
					bullet->setIsMove(false); 
				}
			}
			else
			{
				bullet->setIsMove(true); 
				bullet->SetRect(rect.x + 10, rect.y + 10);
			}
		}
	}
}


void Enemy::RemoveBullet(const int& index)
{
	int size = BulletList.size();
	if (size > 0 && index < size)
	{
		Bullet* bullet = BulletList.at(index);
		BulletList.erase(BulletList.begin() + index);
		if (bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}
}