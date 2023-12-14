#include "Player.h"

Player::Player()
{
	frame = 0; 
	x_pos = 0;
	y_pos = 0;
	x_value = 0; 
	y_value = 0;
	widthFrame = 0;
	heightFrame = 0; 
	movementStatus = WALK_NONE;
	inputType.left = 0; 
	inputType.right = 0;
	inputType.up = 0;
	inputType.down = 0;
	inputType.jump = 0;
	onGround = false;
	mapX = 0;
	mapY = 0;
	reviveTime = 0;
	collectedCoin = 0;
}


Player::~Player()
{
}


bool Player::loadImage(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObjects::loadImage(path, screen);

	if (ret == true)
	{
		widthFrame = rect.w / 8; 
		heightFrame = rect.h;
	}
	return ret; 
}


SDL_Rect Player::GetRectFrame()
{
	SDL_Rect Rect;
	Rect.x = rect.x;
	Rect.y = rect.y; 
	Rect.w = widthFrame; 
	Rect.h = heightFrame;
	return Rect;
}

void Player::setClip()
{
	if (widthFrame > 0 && heightFrame > 0)
	{
		frameClip[0].x = 0; 
		frameClip[0].y = 0;
		frameClip[0].w = widthFrame;
		frameClip[0].h = heightFrame;

		/*for (int i = 1; i < 8; i++)
		{
			frameClip[i].x = i * widthFrame;
			frameClip[i].y = 0;
			frameClip[i].w = widthFrame;
			frameClip[i].h = heightFrame;
		}*/

		frameClip[1].x = widthFrame;
		frameClip[1].y = 0;
		frameClip[1].w = widthFrame;
		frameClip[1].h = heightFrame;

		frameClip[2].x = 2 * widthFrame;
		frameClip[2].y = 0;
		frameClip[2].w = widthFrame;
		frameClip[2].h = heightFrame;

		frameClip[3].x = 3 * widthFrame;
		frameClip[3].y = 0;
		frameClip[3].w = widthFrame;
		frameClip[3].h = heightFrame;

		frameClip[4].x = 4 * widthFrame;
		frameClip[4].y = 0;
		frameClip[4].w = widthFrame;
		frameClip[4].h = heightFrame;

		frameClip[5].x = 5 * widthFrame;
		frameClip[5].y = 0;
		frameClip[5].w = widthFrame;
		frameClip[5].h = heightFrame;

		frameClip[6].x = 6 * widthFrame;
		frameClip[6].y = 0;
		frameClip[6].w = widthFrame;
		frameClip[6].h = heightFrame;

		frameClip[7].x = 7 * widthFrame;
		frameClip[7].y = 0;
		frameClip[7].w = widthFrame;
		frameClip[7].h = heightFrame;
	}
}


void Player::Show(SDL_Renderer* des)
{
	UpdatePlayerImage(des);

	if (inputType.left == 1 || inputType.right == 1)
	{
		frame++; 
	}
	else
	{
		frame = 0; 
	}

	if (frame >= 8)
	{
		frame = 0; 
	}

	if (reviveTime == 0)
	{
		rect.x = x_pos - mapX;
		rect.y = y_pos - mapY;

		SDL_Rect* currentClip = &frameClip[frame];

		SDL_Rect renderQuad = { rect.x, rect.y, widthFrame, heightFrame };

		SDL_RenderCopy(des, object, currentClip, &renderQuad);
	}
}


void Player::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		    {
		    	movementStatus = WALK_RIGHT;
		    	inputType.right = 1;
				inputType.left = 0; 
				UpdatePlayerImage(screen);
		    }
			break;

		case SDLK_LEFT:
		    {
		    	movementStatus = WALK_LEFT;
		    	inputType.left = 1;
				inputType.right = 0;
				UpdatePlayerImage(screen);
		    }
			break;
		}
	}

	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			inputType.right = 0;
		}
		break;

		case SDLK_LEFT:
		{
			inputType.left = 0;
		}
		break;
		}
	}

	if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		if (events.button.button == SDL_BUTTON_RIGHT)
		{
			inputType.jump = 1; 
		}
		else if (events.button.button == SDL_BUTTON_LEFT)
		{
			Bullet* bullet = new Bullet(); 
			bullet->loadImage("textures/pellett.png", screen);

			if (movementStatus == WALK_LEFT)
			{
				bullet->setBulletDir(Bullet::LEFT_DIR);
				bullet->SetRect(this->rect.x, rect.y + heightFrame * 0.25);
			}
			else 
			{
				bullet->setBulletDir(Bullet::RIGHT_DIR);
				bullet->SetRect(this->rect.x + widthFrame - 20, rect.y + heightFrame * 0.25);
			}

			bullet->setX(20);
			bullet->setIsMove(true);

			BulletMag.push_back(bullet);
		}
	}
}


void Player::HandleBullet(SDL_Renderer* des)
{
	for (int i = 0; i < BulletMag.size(); i++)
	{
		Bullet* bullet = BulletMag.at(i);
		if (bullet != nullptr)
		{
			if (bullet->getIsMove() == true)
			{
				bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				bullet->Render(des); 
			}
			else
			{
				BulletMag.erase(BulletMag.begin() + i);
				if (bullet != nullptr)
				{
					delete bullet;
					bullet = nullptr;
				}

			}
		}
	}
}


void Player::RemoveBullet(const int& index)
{
	int size = BulletMag.size(); 
	if (size > 0 && index < size)
	{
		Bullet* bullet = BulletMag.at(index); 
		BulletMag.erase(BulletMag.begin() + index);
		if (bullet)
		{
			delete bullet;
			bullet = nullptr; 
		}
	}
}


void Player::Play(Map& mapData)
{
	if (reviveTime == 0) 
	{
		x_value = 0;
		y_value += 0.8;

		if (y_value >= MAX_FALL_SPEED)
		{
			y_value = MAX_FALL_SPEED;
		}

		if (inputType.left == 1)
		{
			x_value -= PLAYER_SPEED;
		}
		else if (inputType.right == 1)
		{
			x_value += PLAYER_SPEED;
		}

		if (inputType.jump == 1)
		{
			if (onGround == true)
			{
				y_value = -PLAYER_JUMP;
			}
			onGround = false;
			inputType.jump = 0; //after finish jumping 
		}
		CheckToMap(mapData);
		mapMovement(mapData);
	}

	if (reviveTime > 0)
	{
		reviveTime--; 
		if (reviveTime == 0)    //respawn player 
		{
			onGround = false; 
			if (x_pos > 256)
			{
				x_pos -= 256;  //4 tiles map
			}
			else { x_pos = 0; }
		}

		y_pos = 0; 
		x_value = 0; 
		y_value = 0; 
	}
}


void Player::mapMovement(Map& mapData) 
{
	mapData.start_x = x_pos - (SCREEN_WIDTH / 2);
	if (mapData.start_x < 0)
	{
		mapData.start_x = 0; 
	}
	else if (mapData.start_x + SCREEN_WIDTH >= mapData.max_x)
	{
		mapData.start_x = mapData.max_x - SCREEN_WIDTH;
	}

	mapData.start_y = y_pos - (SCREEN_HEIGHT / 2);
	if (mapData.start_y < 0)
	{
		mapData.start_y = 0; 
	}
	else if (mapData.start_y + SCREEN_HEIGHT >= mapData.max_y)
	{
		mapData.start_y = mapData.max_y - SCREEN_HEIGHT;
	}
}


void Player::CheckToMap(Map& mapData)
{
	int x1 = 0; 
	int x2 = 0;

	int y1 = 0;
	int y2 = 0; 


	//Check horizontal
	int heightMin = heightFrame < TILE_SIZE ? heightFrame : TILE_SIZE;

	x1 = (x_pos + x_value) / TILE_SIZE; 
	x2 = (x_pos + x_value + widthFrame - 1) / TILE_SIZE;

	y1 = (y_pos) / TILE_SIZE; 
	y2 = (y_pos + heightMin - 1) / TILE_SIZE; 


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (x_value > 0) //the player are moving to right 
		{
			int val1 = mapData.tile[y1][x2];
			int val2 = mapData.tile[y2][x2];

			if (val1 == COIN || val2 == COIN)
			{
				mapData.tile[y1][x2] = 0; 
				mapData.tile[y2][x2] = 0;
				CoinIncrease(); 
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos = x2 * TILE_SIZE;
					x_pos -= (widthFrame + 1);
					x_value = 0;
				}
			}
		}
		else if (x_value < 0) 
		{
			int val1 = mapData.tile[y1][x1];
			int val2 = mapData.tile[y2][x1];

			if (val1 == COIN || val2 == COIN)
			{
				mapData.tile[y1][x1] = 0;
				mapData.tile[y2][x1] = 0;
				CoinIncrease();
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos = (x1 + 1) * TILE_SIZE;
					x_value = 0;
				}
			}
		}
	}


	//Check vertical 
	int widthMin = widthFrame < TILE_SIZE ? widthFrame : TILE_SIZE; 
	x1 = x_pos / TILE_SIZE;
	x2 = (x_pos + widthMin) / TILE_SIZE;

	y1 = (y_pos + y_value) / TILE_SIZE;
	y2 = (y_pos + y_value + heightFrame - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_value > 0)
		{
			int val1 = mapData.tile[y2][x1];
			int val2 = mapData.tile[y2][x2];

			if (val1 == COIN || val2 == COIN)
			{
				mapData.tile[y2][x1] = 0;
				mapData.tile[y2][x2] = 0;
				CoinIncrease();
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos = y2 * TILE_SIZE;
					y_pos -= (heightFrame + 1);
					y_value = 0;
					onGround = true;

					if (movementStatus == WALK_NONE)
					{
						movementStatus = WALK_RIGHT;
					}
				}
			}
		}
		else if (y_value < 0)
		{
			int val1 = mapData.tile[y1][x1];
			int val2 = mapData.tile[y1][x2];

			if (val1 == COIN || val2 == COIN)
			{
				mapData.tile[y1][x1] = 0;
				mapData.tile[y1][x2] = 0;
				CoinIncrease();
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos = (y1 + 1) * TILE_SIZE;
					y_value = 0;
				}
			}
		}
	}

	x_pos += x_value;
	y_pos += y_value; 

	if (x_pos < 0)
	{
		x_pos = 0; 
	}
	else if (x_pos + widthFrame > mapData.max_x)
	{
		x_pos = mapData.max_x - widthFrame - 1;
	}

	if (y_pos > mapData.max_y)
	{
		reviveTime = 60;
	}
}


void Player::CoinIncrease()
{
	collectedCoin++; 
}


void Player::UpdatePlayerImage(SDL_Renderer* des)
{
	if (onGround == true)
	{
		if (movementStatus == WALK_LEFT)
		{
			loadImage("player sprite/player_left.png", des);
		}
		else 
		{ 
			loadImage("player sprite/player_right.png", des); 
		}
	}
	else
	{
		if (movementStatus == WALK_LEFT)
		{
			loadImage("player sprite/jump_left.png", des);
		}
		else 
		{ 
			loadImage("player sprite/jump_right.png", des); 
		}
	}
}
