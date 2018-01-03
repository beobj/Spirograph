/**
  purpose: This program draws a spirograph using randomly generated
  values in the parametric equations of a hypocycloid. The result is
  interesting to observe.

  @author Benjamin Obaje

  for clarification: www.mathematische-basteleien.de/spirographs.htm
*/
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define SCREENW 900
#define SCREENH 720
#define PI 3.141593

using namespace std;

void drawFilledCirlce(SDL_Renderer* r, int cx, int cy, double radius)
{
	for (int w = 0; w < radius * 2; w++)
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w;
			int dy = radius - h;
			if (dx*dx+ dy*dy <= (radius*radius + radius)
				&& dx*dx + dy*dy <= (radius*radius - radius))
				SDL_RenderDrawPoint(r, cx + dx, cy + dy);
		}
}

void drawSpirograph(SDL_Renderer* ren)
{
	double x, y, r, t;
	int R, a, cycles=70+(rand()%120+1);
	R = 20;
	r = 14;
	a = 12;
	t = cycles * -2 * PI;
	
	//determines the speed of the drawing(increase in angle)
	double steps = 0.01;

	SDL_Event e;
	int red= rand() % 255, g= rand() % 255, b= rand()%255;
	SDL_SetRenderDrawColor(ren, red, g, b, 255);
	
	while (t < pow(t,2))
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym)
				{
				case SDLK_q:
					return;
					break;
				}
		}//handle input while drawing
		x = 10 * ((R - r)*cos((r / R) * t) + a*cos((1 - (r / R))*t));
		y = 10 * ((R - r)*sin((r / R) * t) - a*sin((1 - (r / R))*t));
		drawFilledCirlce(ren, x + SCREENW / 2, y + SCREENH / 2, 1.4);
		SDL_RenderPresent(ren);
		t += steps;
	}//while
}//drawSpirograph

int main(int argc, char* args[])
{
	srand((unsigned int)time(NULL));
	SDL_Window* w = NULL;
	SDL_Renderer* r = NULL;
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	//Create window
	w = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
	if (w == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	//Create renderer for window
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
	if (r == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return -1;
	}
	
	bool quit = false, mouseHeld=false;
	int pointSize = 2;
	//Event handler
	SDL_Event e;

	//set background colour and clear the screen
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderClear(r);

	while (!quit)
	{	
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)	{ quit = true; }
			if (e.type == SDL_KEYDOWN)
			{
				switch(e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_RETURN:
					SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
					SDL_RenderClear(r);
					break;
				case SDLK_a: drawSpirograph(r);	break;
				default:
					break;
				}	
			}// key down
		}//handle input
		//Update screen
		SDL_RenderPresent(r);
	}
	//Destroy window & renderer
	SDL_DestroyWindow(w);
	SDL_DestroyRenderer(r);
	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}