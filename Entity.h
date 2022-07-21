#pragma once
#ifndef ENTYTI_H
#define ENTYTI_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define DEFAULT_SIDE_FRAME 32
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Entity 
{
protected:
	float xPos, yPos;
	float wScale, hScale;
	float angle;
	SDL_Rect frame;
	SDL_Texture* texture;
public:
	Entity(float x_pos, float y_pos, SDL_Texture* tex);
	void setPos(float x_pos, float y_pos);
	void setScale(float w_scale, float h_scale);
	void setAngle(float _angle);
	SDL_Texture* getTexture();
	SDL_Rect getCurrentFrame();
	void update(double timer);

	float getXPos() 
	{
		return xPos;
	}
	float getYPos()
	{
		return yPos;
	}
	float getAngle()
	{
		return angle;
	}
	float getWScale()
	{
		return wScale;
	}
	float getHScale()
	{
		return hScale;
	}
};
#endif // !ENTYTI_H
