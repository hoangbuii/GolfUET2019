#pragma once
#ifndef HOLE_H
#define HOLE_H

#include "Entity.h"

class Hole : public Entity
{
private:
public:
	Hole(float x_pos, float y_pos, SDL_Texture* tex);
};

#endif // !HOLE_H
