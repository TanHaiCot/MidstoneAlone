#pragma once

#ifndef BASE_OBJECTS_H_
#define BASE_OBJECTS_H_

#include "CommonFunctions.h"

class BaseObjects
{
public:
	BaseObjects();
	~BaseObjects(); 
	void SetRect(const int& x, const int& y) { rect.x = x; rect.y = y; }
	SDL_Rect GetRect() const { return rect; }
	SDL_Texture* GetObject() const { return object; }

	virtual bool loadImage(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = nullptr); 
	void Free();
protected:
	SDL_Texture* object; 
	SDL_Rect rect;            // the area of the screen x: 0 -> 1280   y: 0 -> 640
};


#endif
