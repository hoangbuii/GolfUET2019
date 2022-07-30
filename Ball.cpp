#include "Ball.h"

#define PI 3.1415
#define TIME_TO_POLISH -150

Ball::Ball(float x_pos, float y_pos, SDL_Texture* tex, SDL_Texture* directionTex, SDL_Texture* powerMTexFG, SDL_Texture* powerMTexBG)
    :Entity(x_pos, y_pos, tex)
{
    directions.push_back(Entity(-64, -64, directionTex));
    powerBar.push_back(Entity(-64, -64, powerMTexBG));
    powerBar.push_back(Entity(-64, -64, powerMTexFG));
}

void Ball::setWin(bool isWin)
{
    win = isWin;
}

void Ball::setInitialMousePos(float x, float y)
{
    xInitialMousePos = x;
    yInitialMousePos = y;
}

void Ball::setVelocity(float x, float y)
{
    xVelocity = x;
    yVelocity = y;
}

void Ball::setLaunchedVelocity(float x, float y)
{
    xLuanchedVelocity = x;
    yLuanchedVelocity = y;
}

void Ball::update(double timer, bool mouseDown, bool mousePressed, vector<Block> blocks, Hole hole, Mix_Chunk* chargeSfx, Mix_Chunk* swingSfx, Mix_Chunk* holeSfx)
{
    if (win) 
    {
        if (getXPos() < xTarget) 
        {
            float _xPos = getXPos() + (0.1 * timer);
            setPos(_xPos, getYPos());
        }
        else if (getXPos() > xTarget) 
        {
            float _xPos = getXPos() - (0.1 * timer);
            setPos(_xPos, getYPos());
        }

        if (getYPos() < yTarget) 
        {
            float _yPos = getYPos() + (0.1 * timer);
            setPos(getXPos(), _yPos);
        }
        else if (getYPos() > yTarget) 
        {
            float _yPos = getYPos() - (0.1 * timer);
            setPos(getXPos(), _yPos);
        }
        setScale(getWScale() - 0.001 * timer, getHScale() - 0.001 * timer);
        return;
    }


    if (getXPos() + 4 > hole.getXPos() && getXPos() + 16 < hole.getXPos() + 20 && getYPos() + 4 > hole.getYPos() && getYPos() + 16 < hole.getYPos() + 20)
    {
        Mix_PlayChannel(-1, holeSfx, 0);
        setWin(true);
        xTarget = hole.getXPos();
        yTarget = hole.getYPos() + 3;
    }

    if (mousePressed && canMove) 
    {
        Mix_PlayChannel(-1, chargeSfx, 0);
        playedSwingFx = false;
        int xMouse = 0;
        int yMouse = 0;
        SDL_GetMouseState(&xMouse, &yMouse);
        setInitialMousePos(xMouse, yMouse);
    }
    if (mouseDown && canMove)
    {
        int xMouse = 0;
        int yMouse = 0;
        SDL_GetMouseState(&xMouse, &yMouse);
        setVelocity((xMouse - getXInitialMousePos()) / TIME_TO_POLISH, (yMouse - getYInitialMousePos()) / TIME_TO_POLISH);
        setLaunchedVelocity((xMouse - getXInitialMousePos()) / TIME_TO_POLISH, (yMouse - getYInitialMousePos()) / TIME_TO_POLISH);
        velocity = SDL_sqrt(SDL_pow(abs(getXVelocity()), 2) + SDL_pow(abs(getYVelocity()), 2));
        luanchedVelocity = velocity;


        directions.at(0).setPos(getXPos(), getYPos() + 8 - 32);
        directions.at(0).setAngle(SDL_atan2(yVelocity, xVelocity) * (180 / PI) + 90);

        xDirection = xVelocity / abs(xVelocity);
        yDirection = yVelocity / abs(yVelocity);


        if (getXPos() > (SCREEN_WIDTH - DEFAULT_SIDE_FRAME * 4)) {
            powerBar.at(0).setPos(getXPos() - (DEFAULT_SIDE_FRAME + 8), getYPos() - DEFAULT_SIDE_FRAME);
            powerBar.at(1).setPos(getXPos() - (DEFAULT_SIDE_FRAME + 8) + 4, getYPos() + 4  - DEFAULT_SIDE_FRAME * powerBar.at(1).getHScale());
        }
        else {
            powerBar.at(0).setPos(getXPos() + (DEFAULT_SIDE_FRAME + 8), getYPos() - DEFAULT_SIDE_FRAME);
            powerBar.at(1).setPos(getXPos() + (DEFAULT_SIDE_FRAME + 8) + 4, getYPos() + 4 - DEFAULT_SIDE_FRAME * powerBar.at(1).getHScale());
        }

        if (velocity > 1) {
            velocity = 1;
            luanchedVelocity = 1;
        }
        powerBar.at(1).setScale(1, velocity / 1);
    }
    else
    {
        if (!playedSwingFx)
        {
            Mix_PlayChannel(-1, swingSfx, 0);
            playedSwingFx = true;
            strokes++;
        }
        directions.at(0).setPos(-64, -64);
        powerBar.at(0).setPos(-64, -64);
        powerBar.at(1).setPos(-64, -64);
        canMove = false;
        setPos(getXPos() + getXVelocity() * timer, getYPos() + getYVelocity() * timer);

        if (getXVelocity() > 0.0001 || getXVelocity() < -0.0001 || getYVelocity() > 0.0001 || getYVelocity() < -0.0001)
        {

            if (velocity > 0)
            {
                velocity -= friction * timer;
            }
            else
            {
                velocity = 0;
            }
            xVelocity = (velocity / luanchedVelocity) * abs(xLuanchedVelocity) * xDirection;
            yVelocity = (velocity / luanchedVelocity) * abs(yLuanchedVelocity) * yDirection;

        }
        else 
        {
            setVelocity(0, 0);
            int xMouse = 0;
            int yMouse = 0;
            SDL_GetMouseState(&xMouse, &yMouse);
            setInitialMousePos(xMouse, yMouse);
            canMove = true;
        }

        if (getXPos() + getCurrentFrame().w > SCREEN_WIDTH)
        {
            setVelocity(-abs(getXVelocity()), getYVelocity());
            xDirection = -1;
        }
        else if (getXPos() < 0)
        {
            setVelocity(abs(getXVelocity()), getYVelocity());
            xDirection = 1;
        }
        else if (getYPos() + getCurrentFrame().h > SCREEN_HEIGHT)
        {
            setVelocity(getXVelocity(), -abs(getYVelocity()));
            yDirection = -1;
        }
        else if (getYPos() < 0)
        {
            setVelocity(getXVelocity(), abs(getYVelocity()));
            yDirection = 1;
        }


        for (Block& b : blocks)
        {
            float newX = getXPos() + getXVelocity() * timer;
            float newY = getYPos();

            if (newX + 16 > b.getXPos() && newX < b.getXPos() + b.getCurrentFrame().w && newY + 16 > b.getYPos() && newY < b.getYPos() + b.getCurrentFrame().h - 3)
            {
                setVelocity(getXVelocity() * -1, getYVelocity());
                xDirection *= -1;
            }
            newX = getXPos();
            newY = getYPos() + getYVelocity() * timer;

            if (newX + 16 > b.getXPos() && newX < b.getXPos() + b.getCurrentFrame().w && newY + 16 > b.getYPos() && newY < b.getYPos() + b.getCurrentFrame().h - 3)
            {
                setVelocity(getXVelocity(), getYVelocity() * -1);
                yDirection *= -1;
            }
        }
    }

}