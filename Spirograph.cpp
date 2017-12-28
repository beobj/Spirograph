/**
  purpose: This program draws a spirograph using randomly generated(for curiousity)
  values in the parametric equations of a hypocycloid.

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

//draws an unfilled circle 
//using the parametric equations of a circle for simplicity
void drawCircle(SDL_Renderer* r, int x, int y)
{
	double cx, cy;
	int cr = 250;
	for (double i = -PI * 2; i < 0; i += 0.003)
	{
		SDL_SetRenderDrawColor(r, 127, 238, 255, 255);
		cx = cr * cos(i);
		cy = cr * sin(i);
		SDL_RenderDrawPoint(r, cx + x/2, cy + y/2);
		//drawFilledCirlce(r, cx+x*0.5, cy+y*0.5, 2);
	}
	SDL_RenderPresent(r);
}

void drawSpirograph(SDL_Renderer* ren)
{
	double x, y, r, t;
	int R, a, cycles=72+(rand()%120+1);
	R = 4+(rand() % 20+1);
	r = 2+(rand() % 10/1.9f +1);
	a = 2+(rand() % 9+1);
	t = cycles * -2 * PI;
	
	//determines the speed of the drawing(increase in angle)
	double steps = 0.015;

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
		//SDL_RenderDrawPoint(ren, x + SCREENW / 2, y + SCREENH / 2);
		drawFilledCirlce(ren, x + SCREENW / 2, y + SCREENH / 2, 1.24);
		SDL_RenderPresent(ren);
		t += steps;
	}
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
				//case SDLK_d: drawCircle(r, SCREENW, SCREENH);	break;
				case SDLK_a: drawSpirograph(r);	break;
				default:
					break;
				}	
			}// key down
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					int x, y;
					//Get mouse position
					SDL_GetMouseState(&x, &y);
					SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
					drawFilledCirlce(r, x, y, pointSize);
					mouseHeld = true;
				}//mouse click
				if (e.type == SDL_MOUSEMOTION)
				{
					if (mouseHeld)
					{
						int x, y;
						//Get mouse position
						SDL_GetMouseState(&x, &y);
						SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
						drawFilledCirlce(r, x, y, pointSize);
					}
				}//mouse motion
				if (e.type == SDL_MOUSEBUTTONUP) mouseHeld = false;
				if (e.type == SDL_MOUSEWHEEL)
				{
					if (e.wheel.y == 1 && pointSize <= 25)  pointSize++;
					else if (e.wheel.y == -1 && pointSize > 2) pointSize--;
				}
			}//handle continuous mouse input(click+movement)
		}//hanlde input
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