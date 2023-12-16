#include "PlayerHealth&Coin.h"

PlayerHealth::PlayerHealth()
{
	number - 0;
}


PlayerHealth::~PlayerHealth()
{

}


void PlayerHealth::AddPos(const int& xPos)
{
	posList.push_back(xPos);

}


void PlayerHealth::Init(SDL_Renderer* screen)
{
	loadImage("textures/player_pw.png", screen);
	number = 3;
	if (posList.size() > 0)
	{
		posList.clear();
	}

	AddPos(20);
	AddPos(60);
	AddPos(100);
}


void PlayerHealth::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < posList.size(); i++)
	{
		rect.x = posList.at(i);
		rect.y = 0;
		Render(screen);
	}
}


void PlayerHealth::Decrease()
{
	number--;
	posList.pop_back();
}


void PlayerHealth::Increase()
{
	number++;
	int lastPos = posList.back();
	lastPos += 40;
	posList.push_back(lastPos);
}



PlayerCoin::PlayerCoin()
{
	xPOS = 0;
	yPOS = 0;
}


PlayerCoin::~PlayerCoin()
{

}


void PlayerCoin::Init(SDL_Renderer* screen)
{
	bool ret = loadImage("textures/money.png", screen);
}


void PlayerCoin::Show(SDL_Renderer* screen)
{
	rect.x = xPOS;
	rect.y = yPOS;
	Render(screen);
}