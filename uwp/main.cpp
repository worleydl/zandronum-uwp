#include <Windows.h>
#include "SDL2/SDL.h"

extern "C" __declspec(dllimport) void* uwp_GetWindowReference();
extern "C" __declspec(dllimport) int ExternalWinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc);

int bootstrap(int argc, char** argv)
{
	uwp_GetWindowReference(); // Call once to init reference for other threads

	return ExternalWinMain(0, 0, 0, 0); // TODO: Fix args

}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc)
{
	return SDL_WinRTRunApp(bootstrap, NULL);
}
