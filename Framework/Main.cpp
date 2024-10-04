#include "CommonFunctions.h"
#include "BaseObjects.h"
#include "Map.h"
#include "Player.h" 
#include "Timer.h"
#include "Enemy.h"
#include "Text.h"
#include "PlayerHealth&Coin.h"
#include <memory>
#include <iostream>


void* operator new(std::size_t numBytes) {
	std::cout << "allocating " << numBytes << " bytes of memory\n";
	return std::malloc(numBytes);
}

void operator delete(void* memoryLocation, std::size_t numBytes) {
	std::cout << "freeing " << numBytes << " bytes of memory\n";
	std::free(memoryLocation);
}



BaseObjects background; 
TTF_Font* font = nullptr; 

bool InitData()
{
	bool success = true; 
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false; 

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	window = SDL_CreateWindow("Midstone", 
		                       SDL_WINDOWPOS_UNDEFINED,  
		                       SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
		                       SDL_WINDOW_SHOWN);
	 
	if (window == nullptr)
	{
		success = false; 
	}
	else
	{
		screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (screen == nullptr)
		{
			success = false; 
		}
		else
		{
			SDL_SetRenderDrawColor(screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG; 
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false; 
		}

		if (TTF_Init() == -1)
		{
			success = false; 
		}

		font = TTF_OpenFont("Font/dlxfont_.ttf", 15);
		if (font == nullptr)
		{
			success = false; 
		}
	}

	return success; 
}


bool LoadBackground()
{
	bool ret = background.loadImage("textures/Flat_Game_Background_1.jpg", screen);
	if (ret == false)
		return false; 

	return true; 
}


void Close()
{
	background.Free();

	SDL_DestroyRenderer(screen); 
	screen = nullptr; 

	SDL_DestroyWindow(window);
	window = nullptr;

	IMG_Quit();
	SDL_Quit();
}


std::vector<Enemy*> Enemies()
{
	std::vector<Enemy*> enemyArmy;

	Enemy* dynamicEnemies = new Enemy[20]; 
	for (int i = 0; i < 20; i++)
	{
		Enemy* enemy = (dynamicEnemies + i); 
		if (enemy != nullptr)
		{
			enemy->LoadImg("textures/threat_left.png", screen); 
			enemy->SetClip(); 
			enemy->SetMoveType(Enemy::MOVE_IN_AREA); 
			enemy->setXpos(500 + i * 500); 
			enemy->setYpos(200);

			int pos1 = enemy->getXpos() - 60; 
			int pos2 = enemy->getXpos() + 60;
			enemy->SetAnimationPos(pos1, pos2); 
			enemy->SetInputLeft(1);
			enemyArmy.push_back(enemy);		
		}
	}
	Enemy* enemies = new Enemy[20]; 

	for (int i = 0; i < 20; i++)
	{
		Enemy* enemy = (enemies + i);
		if (enemy != nullptr)
		{
			enemy->LoadImg("textures/threat_level.png", screen);
			enemy->SetClip();
			enemy->setXpos(700 + i * 1200);
			enemy->setYpos(250);
			enemy->SetMoveType(Enemy::STATIC_ENEMY); 
			enemy->SetInputLeft(0); 

			Bullet* bullet = new Bullet();
			enemy->InitBullet(bullet, screen);
			enemyArmy.push_back(enemy);
		}
	}

	return enemyArmy; 
}

int main(int argc, char* argv[])
{

	Timer timer; 
	if (InitData() == false)
		return -1; 

	if (LoadBackground() == false)
		return -1; 


	GameMap gameMap; 
	gameMap.loadMap("map/map01.dat");
	gameMap.loadTiles(screen); 


	Player player; 
	player.loadImage("player sprite/player_right.png", screen);
	player.setClip();

	PlayerHealth playerHealth; 
	playerHealth.Init(screen);

	PlayerCoin playerCoin; 
	playerCoin.Init(screen); 

	std::vector<Enemy*> EnemyArmy = Enemies(); 

	int DieTurn = 0; 

	//Time text
	Text gameTime; 
	gameTime.SetColor(Text::WHITE_TEXT); 

	Text gameMark; 
	gameMark.SetColor(Text::WHITE_TEXT);
	UINT markValue = 0; 

	Text gameCoin;
	gameCoin.SetColor(Text::WHITE_TEXT);

	bool isQuit = false;
	while (!isQuit)
	{
		timer.Start();
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				isQuit = true; 

			player.HandleInputAction(event, screen);
		}

		SDL_SetRenderDrawColor(screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(screen);

		background.Render(screen, nullptr); 

		Map mapData = gameMap.getMap(); 

		player.HandleBullet(screen);
		player.setMapXY(mapData.start_x, mapData.start_y);
		player.Play(mapData); 
		player.Show(screen);


		gameMap.setMap(mapData); 
		gameMap.drawMap(screen);

		playerHealth.Show(screen);

		playerCoin.Show(screen); 
		playerCoin.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

		for (int i = 0; i < EnemyArmy.size(); i++)
		{
			Enemy* enemy = EnemyArmy.at(i);
			if (enemy != nullptr)
			{
				enemy->SetMapXY(mapData.start_x, mapData.start_y); 
				enemy->ImpMoveType(screen);
				enemy->Action(mapData);
				enemy->CreateBullet(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				enemy->Show(screen); 

				SDL_Rect playerRect = player.GetRectFrame(); 
				bool Collision1 = false;
				std::vector<Bullet*> Bullet_List = enemy->GetBulletList();
				for (int b = 0; b < Bullet_List.size(); ++b)
				{
					Bullet* bullet = Bullet_List.at(b);
					if (bullet)
					{
						Collision1 = SDLCommonFunctions::CheckCollision(bullet->GetRect(), playerRect); 
						if (Collision1)
						{
							//enemy->RemoveBullet(b);
							break;
						}
					}
				}

				SDL_Rect enemyRect = enemy->GetRectFrame(); 
				bool Collision2 = SDLCommonFunctions::CheckCollision(playerRect, enemyRect);

				if (Collision1 || Collision2)
				{
					DieTurn++;
					if (DieTurn <= 3)
					{
						player.SetRect(0, 0);
						player.SetReviveTime(60);
						playerHealth.Decrease();
						playerHealth.Render(screen);
						SDL_Delay(1000);
						continue;
					}
					else
					{
						/*if (MessageBox(nullptr, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
					    {
						    enemy->Free();
						    Close();
						    SDL_Quit();
						    return 0;
					    }*/
					}
				}
			}
		}

		std::vector<Bullet*> bulletMag = player.getBulletMag(); 
		for (int i = 0; i < bulletMag.size(); ++i)
		{
			Bullet* bullet = bulletMag.at(i);
			if (bullet != nullptr)
			{
				for (int j = 0; j < EnemyArmy.size(); ++j)
				{
					Enemy* enemy = EnemyArmy.at(j); 
					if (enemy != nullptr)
					{
						SDL_Rect eRect; 
						eRect.x = enemy->GetRect().x; 
						eRect.y = enemy->GetRect().y;
						eRect.w = enemy->GetWidthFrame();
						eRect.h = enemy->GetHeightFrame();

						SDL_Rect bRect = bullet->GetRect(); 

						bool Collision = SDLCommonFunctions::CheckCollision(bRect, eRect); 

						if (Collision)
						{
							markValue += 100;
							player.RemoveBullet(i);
							enemy->Free(); 
							EnemyArmy.erase(EnemyArmy.begin() + j);
						}
					}
				}
			}
		}

		//Show game time 
		std::string stringTime = "Time: ";
		Uint32 TimeValue = SDL_GetTicks() / 1000; 
		Uint32 countdownTime = 300 - TimeValue; 
		if (countdownTime <= 0)
		{
			if (MessageBox(nullptr, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				Close();
				SDL_Quit();
				return 0;
			}
		}
		else
		{
			std::string stringValue = std::to_string(countdownTime);
			stringTime += stringValue;

			gameTime.SetText(stringTime);
			gameTime.LoadFromRenderText(font, screen);
			gameTime.RenderText(screen, SCREEN_WIDTH - 200, 15);
		}

		std::string markStringValue = std::to_string(markValue);
		std::string stringMark("Mark: ");
		stringMark += markStringValue;

		gameMark.SetText(stringMark);
		gameMark.LoadFromRenderText(font, screen);
		gameMark.RenderText(screen, SCREEN_WIDTH * 0.5 - 50, 15);

		int coinCount = player.GetCoinCount(); 
		std::string stringCoin = std::to_string(coinCount); 

		gameCoin.SetText(stringCoin);
		gameCoin.LoadFromRenderText(font, screen);
		gameCoin.RenderText(screen, SCREEN_WIDTH * 0.5 - 250, 15);

		SDL_RenderPresent(screen); 

		int realTime = timer.getTick();
		int timePerFrame = 1000 / FRAME_PER_SECOND; //ms

		if (realTime < timePerFrame)
		{
			int delayTime = timePerFrame - realTime; 
			if(delayTime > 0)
				SDL_Delay(delayTime);
		}
	}

	for (int i = 0; i < EnemyArmy.size(); i++)
	{
		Enemy* enemy = EnemyArmy.at(i);
		if (enemy)
		{
			enemy->Free(); 
			enemy = nullptr;
		}
	}

	EnemyArmy.clear(); 

	Close(); 
	return 0; 
}