#include "CommonFunctions.h"
#include "BaseObjects.h"
#include "Map.h"
#include "Player.h" 
#include "Timer.h"
#include "Enemy.h"
#include "Text.h"
#include "PlayerHealth&Coin.h"
#include "PoolAllocator.h"
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <SDL_mixer.h>

BaseObjects background; 
TTF_Font* font = nullptr; 
std::vector<Enemy*> enemyArmy;
std::mutex enemyMutex; 
//std::atomic<bool> assetLoaded(false); 
bool enemiesLoaded = false; 
std::condition_variable cv; 
SDL_GameController* gameController = nullptr; 

bool InitData()
{
	bool success = true; 
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
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

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			gameController = SDL_GameControllerOpen(i);
			if (gameController)
			{
				break;
			}
		}
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		success = false;
	}

	bulletSound = Mix_LoadWAV("game resource/Fire1.wav");
	jumpSound = Mix_LoadWAV("game resource/beep_.wav");
	if (bulletSound == nullptr || jumpSound == nullptr)
	{
		success = false;
	}

	backgroundMusic = Mix_LoadMUS("game resource/05. Loonboon.mp3"); 
	if (backgroundMusic == nullptr)
	{
		success = false; 
	}

	return success; 
}


bool LoadBackground()
{
	bool ret = background.loadImage("textures/Flat_Game_Background_1.jpg", screen);
	if (ret == false)
	{
		std::cout << "Failed to load background" << std::endl; 
		return false;
	}

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

	Mix_FreeMusic(backgroundMusic); 
	backgroundMusic = nullptr;
	Mix_CloseAudio(); 
}

void loadEnemies(std::vector<Enemy*> &enemyArmy)
{

	//Enemy* dynamicEnemies = new Enemy();
	for (int i = 0; i < 20; i++)
	{
		Enemy* enemy = new Enemy(); //(dynamicEnemies + i); 
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
	//Enemy* enemies = new Enemy[20]; 

	for (int i = 0; i < 20; i++)
	{
		Enemy* enemy = new Enemy(); //(enemies + i);
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

	{
		std::lock_guard<std::mutex> lock(enemyMutex); 
		std::cout << "Enemies have been spawned" << std::endl; 
		enemiesLoaded = true;
	}

	cv.notify_one(); 
}

int main(int argc, char* argv[])
{
	if (InitData() == false)
	{
		return -1;
	}

	if (LoadBackground() == false) {
		return -1;
	}

	if (Mix_PlayMusic(backgroundMusic, -1) == -1)
	{
		std::cout << "Failed to play background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}

	GameMap gameMap;
	gameMap.loadMap("map/map01.dat");
	gameMap.loadTiles(screen);
	std::cout << "Game map loaded" << std::endl;

	Player player;
	player.loadImage("player sprite/player_right.png", screen);
	player.setClip();

	PlayerHealth playerHealth;
	playerHealth.Init(screen);

	PlayerCoin playerCoin;
	playerCoin.Init(screen);

	std::cout << "Starting enemy thread" << std::endl;
	std::thread enemyThread(loadEnemies, std::ref(enemyArmy));
	{
		std::unique_lock<std::mutex> lock(enemyMutex);
		cv.wait(lock, [] {return enemiesLoaded; });
	}

	int DieTurn = 0;

	//Time text
	Text gameTime;
	gameTime.SetColor(Text::WHITE_TEXT);

	Text gameMark;
	gameMark.SetColor(Text::WHITE_TEXT);
	UINT markValue = 0;

	Text gameCoin;
	gameCoin.SetColor(Text::WHITE_TEXT);

	Text gameFPS;
	gameFPS.SetColor(Text::WHITE_TEXT);

	Timer gameTimer;
	Timer frameTimer;

	gameTimer.Start();

	bool isQuit = false;
	while (!isQuit)
	{
		frameTimer.Start();

		bool controllerActivated = false; 

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				isQuit = true;


			player.HandleInputAction(event, screen, bulletSound, jumpSound);
			if (SDL_GameControllerGetAttached(gameController))
			{
				controllerActivated = true;
				player.HandleGameControllerInput(gameController, screen, bulletSound, jumpSound);
			}
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

		{
			std::lock_guard<std::mutex> lock(enemyMutex);
			for (int i = 0; i < enemyArmy.size(); i++)
			{
				Enemy* enemy = enemyArmy.at(i);
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
				{
					std::lock_guard<std::mutex> lock(enemyMutex);
					for (int j = 0; j < enemyArmy.size(); ++j)
					{
						Enemy* enemy = enemyArmy.at(j);
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
								enemyArmy.erase(enemyArmy.begin() + j);
							}
						}
					}
				}
			}
		}

		double frameTime = frameTimer.GetElapsedMilliseconds();
		double fps = 1000.0 / frameTime;
		std::cout << "Frame Time: " << frameTime << " ms, FPS: " << fps << std::endl;

		double targetFrameTime = 1000.0 / FRAME_PER_SECOND; //ms
		if (frameTime < targetFrameTime)
		{
			SDL_Delay(static_cast<Uint32>((targetFrameTime - frameTime)));
		}

		//Show game time 
		double gameTimeValue = gameTimer.GetElapsedMilliseconds();
		double countdownTime = 300.0 * 1000.0 - gameTimeValue;
		//std::string stringTime = "Time: ";
		//Uint32 TimeValue = SDL_GetTicks() / 1000; 
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
			std::string stringTime = "Time" + std::to_string(static_cast<int>(countdownTime));
			gameTime.SetText(stringTime);
			gameTime.LoadFromRenderText(font, screen);
			gameTime.RenderText(screen, SCREEN_WIDTH - 200, 15);
		}

		std::string stringMark = "Mark: " + std::to_string(markValue);
		gameMark.SetText(stringMark);
		gameMark.LoadFromRenderText(font, screen);
		gameMark.RenderText(screen, SCREEN_WIDTH * 0.5 - 50, 15);

		int coinCount = player.GetCoinCount();
		std::string stringCoin = std::to_string(coinCount);
		gameCoin.SetText(stringCoin);
		gameCoin.LoadFromRenderText(font, screen);
		gameCoin.RenderText(screen, SCREEN_WIDTH * 0.5 - 250, 15);

		//int realTime = timer.getTick();
		std::string StringFPS = std::to_string(static_cast<int>(fps));
		gameFPS.SetText(StringFPS);
		gameFPS.LoadFromRenderText(font, screen);
		gameFPS.RenderText(screen, 10, 15);

		SDL_RenderPresent(screen);

	}

	
	
	enemyThread.join(); 

	for (int i = 0; i < enemyArmy.size(); i++)
	{
		Enemy* enemy = enemyArmy.at(i);
		if (enemy)
		{
			enemy->Free(); 
			delete enemy; 
			enemy = nullptr;
		}
	}

	enemyArmy.clear(); 

	Close(); 
	SDL_GameControllerClose(gameController);
	std::cout << "Program ended" << std::endl;
	return 0; 
}