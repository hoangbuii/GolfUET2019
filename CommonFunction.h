#pragma once
#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include "Entity.h"

#define TITLE "GolfUET2019"

using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init() 
{
	bool success = true;
	int img_flags = IMG_INIT_PNG;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Failed to initiate SDL: " << SDL_GetError() << endl;
		success = false;
	}
	if (TTF_Init() < 0)
	{
		cout << "Failed to load SDL_ttf: " << TTF_GetError() << endl;
		success = false;
	}
	if (!(IMG_Init(img_flags) & img_flags))
	{
		cout << "Failed to load SDL_image: " << IMG_GetError() << endl;
		success = false;
	}
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	gWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		cout << "Failed to create window: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL)
		{
			cout << "Failed to create window: " << SDL_GetError() << endl;
			success = false;
		}
	}
	return success;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

SDL_Texture* loadTexture(const char* filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(gRenderer, filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;

	return texture;
}

void render(Entity entity)
{
	SDL_Rect src;
	src.x = entity.getCurrentFrame().x;
	src.y = entity.getCurrentFrame().y;
	src.w = entity.getCurrentFrame().w;
	src.h = entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = entity.getXPos() + (entity.getCurrentFrame().w - entity.getCurrentFrame().w * entity.getWScale()) / 2;
	dst.y = entity.getYPos() + (entity.getCurrentFrame().h - entity.getCurrentFrame().h * entity.getHScale()) / 2;
	dst.w = entity.getCurrentFrame().w * entity.getWScale();
	dst.h = entity.getCurrentFrame().h * entity.getHScale();

	SDL_RenderCopyEx(gRenderer, entity.getTexture(), &src, &dst, entity.getAngle(), 0, SDL_FLIP_NONE);
}

void render(int x, int y, SDL_Texture* texture)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;

	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(gRenderer, texture, &src, &dst);
}

void renderMessage(float x, float y, const char* text, TTF_Font* font, SDL_Color textColor)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = surfaceMessage->w;
	src.h = surfaceMessage->h;

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(gRenderer, message, &src, &dst);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void renderCenter(float x, float y, const char* text, TTF_Font* font, SDL_Color textColor)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = surfaceMessage->w;
	src.h = surfaceMessage->h;

	SDL_Rect dst;
	dst.x = 640 / 2 - src.w / 2 + x;
	dst.y = 480 / 2 - src.h / 2 + y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(gRenderer, message, &src, &dst);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void display()
{
	SDL_RenderPresent(gRenderer);
}

#endif // !COMMON_FUNCTION_H