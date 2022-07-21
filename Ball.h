#pragma once
#ifndef BALL_H
#define BALL_H

#include "Entity.h"
#include "Block.h"
#include "Hole.h"
#include <vector>

using namespace std;

class Ball : public Entity 
{
private:
	float xVelocity, yVelocity, velocity;
	float xTarget, yTarget;
	float xLuanchedVelocity, yLuanchedVelocity, luanchedVelocity;
	float xInitialMousePos, yInitialMousePos;
    bool canMove = true;
    bool playedSwingFx = true;
    int strokes = 0;
    int xDirection = 1;
    int yDirection = 1;
    bool win = false;
    float friction = 0.001;
    vector<Entity> directions;
    vector<Entity> powerBar;
public:
    Ball(float x_pos, float y_pos, SDL_Texture* tex, SDL_Texture* directionTex, SDL_Texture* powerMTexFG, SDL_Texture* powerMTexBG);
    void setWin(bool isWin);
    void setInitialMousePos(float x, float y);
    void setVelocity(float x, float y);
    void setLaunchedVelocity(float x, float y);
    void update(double timer, bool mouseDown, bool mousePressed, vector<Block> blocks, Hole hole, Mix_Chunk* chargeSfx, Mix_Chunk* swingSfx, Mix_Chunk* holeSfx);

    float getXVelocity() 
    {
        return xVelocity;
    }

    float getYVelocity() 
    {
        return yVelocity;
    }

    float getXInitialMousePos() 
    {
        return xInitialMousePos;
    }

    float getYInitialMousePos() 
    {
        return yInitialMousePos;
    }

    float getXLuanchedVelocity() 
    {
        return xLuanchedVelocity;
    }

    float getYLuanchedVelocity() 
    {
        return yLuanchedVelocity;
    }

    vector<Entity> getDirections()
    {
        return directions;
    }

    vector<Entity> getPowerBar()
    {
        return powerBar;
    }

    int getStrokes()
    {
        return strokes;
    }

    bool isWin()
    {
        return win;
    }
};

#endif // !BALL_H
