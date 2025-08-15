#include <Windows.h>
#include "SDL2/SDL.h"

extern "C" __declspec(dllimport) void* uwp_GetWindowReference();
extern "C" __declspec(dllimport) int ExternalWinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc);

static HINSTANCE hInst;
static HINSTANCE prev_hInst;

int bootstrap(int argc, char** argv)
{
	uwp_GetWindowReference(); // Call once to init reference for other threads

	return ExternalWinMain(hInst, prev_hInst, 0, 0); // TODO: Fix cmd line args
}

int CALLBACK WinMain(HINSTANCE h, HINSTANCE prev, LPSTR argv, int argc)
{
	hInst = h;
	prev_hInst = prev;

	return SDL_WinRTRunApp(bootstrap, NULL);
}
