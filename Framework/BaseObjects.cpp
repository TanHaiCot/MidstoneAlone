#include "BaseObjects.h"

BaseObjects::BaseObjects()
{
	object = nullptr;
	rect.x = 0;
	rect.y = 0;
	rect.w = 0;
	rect.h = 0;
}


BaseObjects::~BaseObjects()
{
}



bool BaseObjects::loadImage(std::string path, SDL_Renderer* screen)
{
	Free();
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadSurface = IMG_Load(path.c_str());
	if (loadSurface != nullptr)
	{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		newTexture = SDL_CreateTextureFromSurface(screen, loadSurface);
		if (newTexture != nullptr)
		{
			rect.w = loadSurface->w;
			rect.h = loadSurface->h;
		}

		SDL_FreeSurface(loadSurface);
	}

	object = newTexture;

	return object != nullptr;
}


void BaseObjects::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
	SDL_Rect renderquad = { rect.x, rect.y, rect.w, rect.h };
	SDL_RenderCopy(des, object, clip, &renderquad);
}


void BaseObjects::Free()
{
	if (object != nullptr)
	{
		SDL_DestroyTexture(object);
		object = nullptr;
		rect.w = 0;
		rect.h = 0;
	}
}