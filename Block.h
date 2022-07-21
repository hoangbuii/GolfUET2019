#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include "Entity.h"

class Block : public Entity
{
private:
public:
	Block(float x_pos, float y_pos, SDL_Texture* tex);
};

#endif // !BLOCK_H
