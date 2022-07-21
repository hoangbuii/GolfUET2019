#include "Entity.h"

Entity::Entity(float x_pos, float y_pos, SDL_Texture* tex)
	:xPos(x_pos), yPos(y_pos), texture(tex)
{
	setScale(1, 1);
	setAngle(0);
	frame.x = 0;
	frame.y = 0;
	frame.w = DEFAULT_SIDE_FRAME;
	frame.h = DEFAULT_SIDE_FRAME;
	SDL_QueryTexture(tex, NULL, NULL, &frame.w, &frame.h);
}

void Entity::setPos(float x_pos, float y_pos)
{
	xPos = x_pos;
	yPos = y_pos;
}

void Entity::setScale(float w_scale, float h_scale)
{
	wScale = w_scale;
	hScale = h_scale;
}

void Entity::setAngle(float _angle)
{
	angle = _angle;
}

SDL_Texture* Entity::getTexture()
{
	return texture;
}

SDL_Rect Entity::getCurrentFrame()
{
	return frame;
}