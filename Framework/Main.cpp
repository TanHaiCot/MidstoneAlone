#include "CommonFunctions.h"
#include "BaseObjects.h"
#include "Map.h"
#include "Player.h" 
#include "Timer.h"
#include "Enemy.h"
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
#include "imgui.h"
//#include "imgui-master/backends/imgui_impl_sdl2.h"
//#include "imgui-master/backends/imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


BaseObjects background; 
std::vector<Enemy*> enemyArmy;
std::mutex enemyMutex; 
//std::atomic<bool> assetLoaded(false); 
bool enemiesLoaded = false; 
std::condition_variable cv; 
SDL_GameController* gameController = nullptr; 
int menuSelection = 0; 
bool showOptions = false;
Uint32 lastInputTime = 0;

enum class GameState {
	MENU,
	GAMEPLAY,
	EXIT
};

void RenderMenu(GameState& gameState, bool& options, int menuSelection) {
	ImGui_ImplSDLRenderer2_NewFrame(); 
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame(); 

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

	// Calculate window position to center it
	ImVec2 window_size = ImVec2(300, 200);
	ImVec2 window_pos = ImVec2((SCREEN_WIDTH - window_size.x) * 0.5f,
		(SCREEN_HEIGHT - window_size.y) * 0.5f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

	ImGui::Begin("Main Menu", nullptr, window_flags);
	ImVec4 highlightColor = ImVec4(0.4f, 0.7f, 0.9f, 1.0f);
	
	if (menuSelection == 0) {
		ImGui::PushStyleColor(ImGuiCol_Button, highlightColor);
	}
	if (ImGui::Button("Start Game", ImVec2(280, 40))) {
		gameState = GameState::GAMEPLAY;
	}
	if (menuSelection == 0)
		ImGui::PopStyleColor();
	ImGui::Spacing();

	if (menuSelection == 1) {
		ImGui::PushStyleColor(ImGuiCol_Button, highlightColor);
	}
	if (ImGui::Button("Options", ImVec2(280, 40))) {
		showOptions = true;
	}
	if (menuSelection == 1)
		ImGui::PopStyleColor();
	ImGui::Spacing();

	if (menuSelection == 2) {
		ImGui::PushStyleColor(ImGuiCol_Button, highlightColor);
	}	
	if (ImGui::Button("Quit", ImVec2(280, 40))) {
		gameState = GameState::EXIT;
	}
	if (menuSelection == 2	)
		ImGui::PopStyleColor();

	ImGui::End();

	if (showOptions) {
		ImGui::Begin("Options", &showOptions, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		static float volume = 0.5f;
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f)) {
			Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
		}

		static bool fullscreen = false;
		if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
			if (fullscreen) {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			else {
				SDL_SetWindowFullscreen(window, 0);
			}
		}

		if (ImGui::Button("Close", ImVec2(280, 40))) {
			showOptions = false;
		}

		ImGui::End();
	}

	ImGui::Render();	
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), screen);
}

bool InitData()
{
	bool success = true; 
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	if (ret < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GameControllerEventState(SDL_ENABLE); 

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	window = SDL_CreateWindow("Midstone", 
		                       SDL_WINDOWPOS_UNDEFINED,  
		                       SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
		                       SDL_WINDOW_SHOWN);
	 
	if (window == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		success = false; 
	}
	else
	{
		screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (screen == nullptr)
		{
			std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			success = false; 
		}
		else
		{
			SDL_SetRenderDrawColor(screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG; 
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
				success = false;
			}
		}
	}

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			gameController = SDL_GameControllerOpen(i);
			if (gameController)
			{
				std::cout << "Game controller " << i << " connected: " << SDL_GameControllerName(gameController) << std::endl;
				break;
			}
			else
			{
				std::cout << "Could not open game controller " << i << "! SDL_Error: " << SDL_GetError() << std::endl;
			}
		}
	}

	if (!gameController)
	{
		std::cout << "No game controller detected." << std::endl;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}

	bulletSound = Mix_LoadWAV("game resource/Fire1.wav");
	jumpSound = Mix_LoadWAV("game resource/beep_.wav");
	if (bulletSound == nullptr || jumpSound == nullptr)
	{
		std::cout << "Failed to load sound effects! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}

	backgroundMusic = Mix_LoadMUS("game resource/05. Loonboon.mp3"); 
	if (backgroundMusic == nullptr)
	{
		std::cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false; 
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark(); // or ImGui::StyleColorsClassic();

	// Initialize ImGui SDL2 and SDL_Renderer bindings
	if (!ImGui_ImplSDL2_InitForSDLRenderer(window, screen)) {
		std::cout << "Failed to initialize ImGui SDL2" << std::endl;
		success = false;
	}
	if (!ImGui_ImplSDLRenderer2_Init(screen)) {
		std::cout << "Failed to initialize ImGui SDL_Renderer" << std::endl;
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

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
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
	Timer profilingTimer; 

	profilingTimer.Start(); 
	if (InitData() == false)
	{
		return -1;
	}
	double initDataTime = profilingTimer.GetElapsedMilliseconds(); 
	std::cout << "InitData() took: " << initDataTime << " ms" << std::endl; 


	profilingTimer.Start();
	if (LoadBackground() == false) {
		return -1;
	}
	double loadBackgroundTime = profilingTimer.GetElapsedMilliseconds();
	std::cout << "LoadBackground() took: " << loadBackgroundTime << " ms" << std::endl;


	if (Mix_PlayMusic(backgroundMusic, -1) == -1)
	{
		std::cout << "Failed to play background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}


	GameMap gameMap;
	profilingTimer.Start();
	gameMap.loadMap("map/map01.dat");
	gameMap.loadTiles(screen);
	double mapLoadTime = profilingTimer.GetElapsedMilliseconds();
	std::cout << "Loading and drawing map took: " << mapLoadTime << " ms" << std::endl;
	//std::cout << "Game map loaded" << std::endl;

	Player player;
	player.loadImage("player sprite/player_right.png", screen);
	player.setClip();

	PlayerHealth playerHealth;
	playerHealth.Init(screen);

	PlayerCoin playerCoin;
	playerCoin.Init(screen);

	std::cout << "Starting enemy thread" << std::endl;
	profilingTimer.Start();
	std::thread enemyThread(loadEnemies, std::ref(enemyArmy));
	{
		std::unique_lock<std::mutex> lock(enemyMutex);
		cv.wait(lock, [] {return enemiesLoaded; });
	}
	double loadEnemiesTime = profilingTimer.GetElapsedMilliseconds();
	std::cout << "loadEnemies() took: " << loadEnemiesTime << " ms" << std::endl;

	int DieTurn = 0;

	Timer gameTimer;
	Timer frameTimer;

	gameTimer.Start();

	bool isQuit = false;
	GameState gameState = GameState::MENU;

	while (!isQuit)
	{
		frameTimer.Start();

		bool controllerActivated = false; 

		while (SDL_PollEvent(&event) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

			if (event.type == SDL_QUIT)
				isQuit = true;

			if (event.type == SDL_CONTROLLERDEVICEADDED)
			{
				if (!gameController)
				{
					gameController = SDL_GameControllerOpen(event.cdevice.which);
					if (gameController)
					{
						std::cout << "Controller connected: " << SDL_GameControllerName(gameController) << std::endl;
					}
				}
			}
			else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
			{
				if (gameController && event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController)))
				{
					SDL_GameControllerClose(gameController);
					gameController = nullptr; 
					std::cout << "Controller disconnected." << std::endl;
				}
			}

			if (gameState == GameState::GAMEPLAY) {
				player.HandleInputAction(event, screen, bulletSound, jumpSound);
			}
		}

		if (gameState == GameState::GAMEPLAY) {
			if (gameController && SDL_GameControllerGetAttached(gameController))
			{
				if (gameController) {
					std::cout << "gameController is valid." << std::endl;
					if (SDL_GameControllerGetAttached(gameController)) {
						player.HandleGameControllerInput(gameController, screen, bulletSound, jumpSound);
					}
					else {
						std::cout << "Controller is not attached during gameplay." << std::endl;
					}
				}
				else {
					std::cout << "gameController is nullptr." << std::endl;
				}
				
				//controllerActivated = true;
				
			}
		}
		
		if (gameState == GameState::MENU && gameController)
		{

			Uint32 currentTime = SDL_GetTicks();
			if (currentTime - lastInputTime > 200) // debounce time in ms
			{
				// Handle D-pad or left stick for navigation
				Sint16 leftY = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);
				const int DEADZONE = 8000;

				if (leftY < -DEADZONE)
				{
					menuSelection--;
					if (menuSelection < 0)
						menuSelection = 2;
					std::cout << "Menu Selection: " << menuSelection << std::endl;
					lastInputTime = currentTime;
				}
				else if (leftY > DEADZONE)
				{
					menuSelection++;
					if (menuSelection > 2)
						menuSelection = 0;
					std::cout << "Menu Selection: " << menuSelection << std::endl;
					lastInputTime = currentTime;
				}
			}

			if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A))
			{
				Uint32 currentTime = SDL_GetTicks(); 
				// To prevent multiple triggers, check if enough time has passed
				if (currentTime - lastInputTime > 200)
				{
					std::cout << "A button pressed on menuSelection: " << menuSelection << std::endl;
					if (menuSelection == 0)
					{
						gameState = GameState::GAMEPLAY;
					}
					else if (menuSelection == 1)
					{
						showOptions = true;
					}
					else if (menuSelection == 2)
					{
						isQuit = true;
						exit(0);
					}
					lastInputTime = currentTime;
				}
			}
		}

		SDL_SetRenderDrawColor(screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(screen);

		if (gameState == GameState::MENU) {
			RenderMenu(gameState, showOptions, menuSelection);
		}

		else if (gameState == GameState::GAMEPLAY) {
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
								//markValue += 100;
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
		//std::cout << "Frame Time: " << frameTime << " ms, FPS: " << fps << std::endl;

		double targetFrameTime = 1000.0 / FRAME_PER_SECOND; //ms
		if (frameTime < targetFrameTime)
		{
			SDL_Delay(static_cast<Uint32>((targetFrameTime - frameTime)));
		}

		//Show game time 
		double gameTimeValue = gameTimer.GetElapsedMilliseconds();
		double countdownTime = 300.0 * 1000.0 - gameTimeValue;
		
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
	SDL_Quit();
	std::cout << "Program ended" << std::endl;
	return 0; 
}