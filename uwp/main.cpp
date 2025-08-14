#include <Windows.h>
#include "SDL2/SDL.h"

extern "C" __declspec(dllimport) void* uwp_GetWindowReference();
extern "C" __declspec(dllimport) int ExternalWinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc);

static HINSTANCE hInst;
static HINSTANCE prev_hInst;

int bootstrap(int argc, char** argv)
{
	uwp_GetWindowReference(); // Call once to init reference for other threads

	// Placeholder mesa bridge code
	// In SDL_BRIDGE mode mesa tries to fetch the current framebuffer size from SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* tmpBridge = SDL_CreateWindow("", 0, 0, 800, 600, 0); // BUG: Init size gets DESTROYED by fullscreen values no matter the flag value
	SDL_SetWindowSize(tmpBridge, 800, 600); // This will bypass the bug above and get mesa to resize the buffer

	int ret = ExternalWinMain(hInst, prev_hInst, 0, 0); // TODO: Fix cmd line args

	SDL_DestroyWindow(tmpBridge);
	SDL_Quit();

	return ret;
}

int CALLBACK WinMain(HINSTANCE h, HINSTANCE prev, LPSTR argv, int argc)
{
	hInst = h;
	prev_hInst = prev;

	return SDL_WinRTRunApp(bootstrap, NULL);
}
