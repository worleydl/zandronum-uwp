#include <Windows.h>
#include "SDL2/SDL.h"

extern "C" __declspec(dllimport) void* uwp_GetWindowReference();
extern "C" __declspec(dllimport) int ExternalWinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc);

static HINSTANCE hInst;
static HINSTANCE prev_hInst;

int bootstrap(int argc, char** argv)
{
	uwp_GetWindowReference(); // Call once to init reference for other threads

	// Ugly SDL bridge workaround, may revert to different version since project doesn't currently use SDL
	// The current mesa build pings SDL looking for the requested window size, it should have a failsafe not sure what happened
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* bridgeWorkaround = SDL_CreateWindow("", 0, 0, 1920, 1080, 0);

	return ExternalWinMain(hInst, prev_hInst, 0, 0); // TODO: Fix args

}

int CALLBACK WinMain(HINSTANCE h, HINSTANCE prev, LPSTR argv, int argc)
{
	hInst = h;
	prev_hInst = prev;

	return SDL_WinRTRunApp(bootstrap, NULL);
}
