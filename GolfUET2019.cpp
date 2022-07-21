#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "CommonFunction.h"
#include "Entity.h"
#include "Ball.h"	
#include "Block.h"
#include "Hole.h"

#define MAX_LEVEL 3

using namespace std;

bool SDlInit = init();

SDL_Texture* ballTexture = loadTexture("assets/gfx/ball.png");
SDL_Texture* holeTexture = loadTexture("assets/gfx/hole.png");
SDL_Texture* pointTexture = loadTexture("assets/gfx/point.png");
SDL_Texture* ballShadowTexture = loadTexture("assets/gfx/ball_shadow.png");


SDL_Texture* blockDarkTexture32 = loadTexture("assets/gfx/block32_dark.png");
SDL_Texture* blockDarkTexture64 = loadTexture("assets/gfx/block64_dark.png");
SDL_Texture* blockLightTexture32 = loadTexture("assets/gfx/block32_light.png");
SDL_Texture* blockLightTexture64 = loadTexture("assets/gfx/block64_light.png");
SDL_Texture* blockTexture32 = blockDarkTexture32;
SDL_Texture* blockTexture64 = blockDarkTexture64;

SDL_Texture* bgTextureL = loadTexture("assets/gfx/bgl.png");
SDL_Texture* bgTextureD = loadTexture("assets/gfx/bgd.png");
SDL_Texture* bgTexture = bgTextureL;

SDL_Texture* uiBgTexture = loadTexture("assets/gfx/UI_bg.png");
SDL_Texture* levelTextBgTexture = loadTexture("assets/gfx/levelText_bg.png");
SDL_Texture* powerMeterTexture_FG = loadTexture("assets/gfx/powermeter_fg.png");
SDL_Texture* powerMeterTexture_BG = loadTexture("assets/gfx/powermeter_bg.png");
SDL_Texture* powerMeterTexture_overlay = loadTexture("assets/gfx/powermeter_overlay.png");
SDL_Texture* logoTexture = loadTexture("assets/gfx/logo.png");
SDL_Texture* click2start = loadTexture("assets/gfx/click2start.png");
SDL_Texture* endscreenOverlayTexture = loadTexture("assets/gfx/end.png");
SDL_Texture* splashBgTexture = loadTexture("assets/gfx/splashbg.png");


Mix_Chunk* chargeSfx = Mix_LoadWAV("assets/sfx/charge.mp3");
Mix_Chunk* swingSfx = Mix_LoadWAV("assets/sfx/swing.mp3");
Mix_Chunk* holeSfx = Mix_LoadWAV("assets/sfx/hole.mp3");


SDL_Color white = { 255, 255, 255 };
SDL_Color black = { 0, 0, 0 };
TTF_Font* font32 = TTF_OpenFont("assets/font/font.ttf", 32);
TTF_Font* font48 = TTF_OpenFont("assets/font/font.ttf", 48);
TTF_Font* font24 = TTF_OpenFont("assets/font/font.ttf", 24);

Ball ball(0, 0, ballTexture, pointTexture, powerMeterTexture_FG, powerMeterTexture_BG);
Hole hole(0, 0, holeTexture);

int level = 0;
vector<Block> blocks = {};

bool gameRunning = true;
bool mouseDown = false;
bool mousePressed = false;

bool swingPlayed = false;
bool secondSwingPlayed = false;

SDL_Event event;


enum STATE {INTRO, PLAYING, FINISH};
int state = INTRO;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;

Block newBlock(int type, int x, int y) {
	
	Block tempBlock(DEFAULT_SIDE_FRAME, DEFAULT_SIDE_FRAME, blockTexture64);
	if (type == 32) {
		tempBlock = Block(DEFAULT_SIDE_FRAME * x, DEFAULT_SIDE_FRAME * y, blockTexture32);
	}
	else {
		tempBlock = Block(DEFAULT_SIDE_FRAME * x, DEFAULT_SIDE_FRAME * y, blockTexture64);
	}

	return tempBlock;
}

void loadLevel(int level) 
{
	if (level > MAX_LEVEL) {
		state = FINISH;
		return;
	}
	string fileLevel = "levels/level" + to_string(level) + ".txt";
	fstream fLevel;
	fLevel.open(fileLevel.c_str());
	if (!fLevel.is_open()) {
		cout << "Failed to load level " << level << "!";
	}
	else {
		if (level % 2) {
			bgTexture = bgTextureD;
			blockTexture32 = blockLightTexture32;
			blockTexture64 = blockLightTexture64;
		}
		else {
			bgTexture = bgTextureL;
			blockTexture32 = blockDarkTexture32;
			blockTexture64 = blockDarkTexture64;
		}
		int type, tx, ty;
		fLevel >> tx >> ty;
		ball.setPos(tx, ty);
		ball.setScale(1, 1);
		fLevel >> tx >> ty;
		ball.setWin(false);
		ball.setVelocity(0, 0);
		hole.setPos(tx, ty);
		hole.setScale(1, 1);
		blocks.clear();
		while (fLevel && !fLevel.eof()) {
			fLevel >> type >> tx >> ty;
			blocks.push_back(newBlock(type, tx, ty));
		}
	}
}

string getStrokeText()
{

	
	return "STROKES: " + to_string(ball.getStrokes());
}

string getLevelText()
{
	return "HOLE: " + to_string(level);
}

void update()
{

	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double)((currentTick - lastTick) * 1000 / (double)SDL_GetPerformanceFrequency());

	mousePressed = false;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mouseDown = true;
				mousePressed = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mouseDown = false;
			}
			break;
		}
	}

	if (state == PLAYING)
	{
		ball.update(deltaTime, mouseDown, mousePressed, blocks, hole, chargeSfx, swingSfx, holeSfx);
		if (ball.getWScale() < -1)
		{
			level++;
			loadLevel(level);
		}
	}
}

void graphics()
{
	SDL_RenderClear(gRenderer);
	render(0, 0, bgTexture);
	render(hole);
	render(ball);
	for (Block b : blocks) 
	{
		render(b);
	}

	for (Entity e : ball.getPowerBar())
	{
		render(e);
	}

	render(ball.getPowerBar().at(0).getXPos(), ball.getPowerBar().at(0).getYPos(), powerMeterTexture_overlay);
	render(ball.getDirections().at(0));
	
	if (state != FINISH)
	{
		render(640 / 2 - 132 / 2, 480 - 32, levelTextBgTexture);
		renderCenter(0, 240 - 16 + 3, getLevelText().c_str(), font24, black);
		renderCenter(0, 240 - 16, getLevelText().c_str(), font24, white);

		render(640 / 2 - 196 / 2, 0, uiBgTexture);
		renderCenter(0, -240 + 16 + 3, getStrokeText().c_str(), font24, black);
		renderCenter(0, -240 + 16, getStrokeText().c_str(), font24, white);
	}
	else
	{
		render(0, 0, endscreenOverlayTexture);
		renderCenter(0, 3 - 32, "YOU COMPLETED THE COURSE!", font48, black);
		renderCenter(0, -32, "YOU COMPLETED THE COURSE!", font48, white);
		renderCenter(0, 3 + 32, getStrokeText().c_str(), font32, black);
		renderCenter(0, 32, getStrokeText().c_str(), font32, white);
	}
	display();
}

void introScreen()
{
	if (SDL_GetTicks() < 1500)
	{
		if (!swingPlayed)
		{
			Mix_PlayChannel(-1, swingSfx, 0);
			swingPlayed = true;
		}
		//Get our controls and events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				gameRunning = false;
				break;
			}
		}

		SDL_RenderClear(gRenderer);
		render(0, 0, bgTexture);
		render(0, 0, splashBgTexture);
		renderCenter(0, 0 + 3, "LOADING...", font32, black);
		renderCenter(0, 0, "LOADING...", font32, white);
		display();
	}
	else
	{
		if (!secondSwingPlayed)
		{
			Mix_PlayChannel(-1, swingSfx, 0);
			secondSwingPlayed = true;
		}
		lastTick = currentTick;
		currentTick = SDL_GetPerformanceCounter();
		deltaTime = (double)((currentTick - lastTick) * 1000 / (double)SDL_GetPerformanceFrequency());

		//Get our controls and events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				gameRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					Mix_PlayChannel(-1, holeSfx, 0);
					state = PLAYING;
				}
				break;
			}
		}
		SDL_RenderClear(gRenderer);
		render(0, 0, bgTexture);
		render(320 - 160, 240 - 100 - 50 + 4 * SDL_sin(SDL_GetTicks() * (3.14 / 1500)), logoTexture);
		render(0, 0, click2start);
		renderCenter(0, 240 - 48 + 3 - 16 * 5, "LEFT CLICK TO START", font32, black);
		renderCenter(0, 240 - 48 - 16 * 5, "LEFT CLICK TO START", font32, white);
		display();
	}
}


void game()
{
	if (state == INTRO)
	{
		introScreen();
	}
	else
	{
		graphics();
		update();
		
	}
}

int main(int argc, char* args[])
{
	
	loadLevel(level);
	
	while (gameRunning)
	{

		game();
	}
	
	
	close();
	return 0;
}