#include <Windows.h>
#include "SDL2/SDL.h"

#include "bootmenu.h"


extern "C" __declspec(dllimport) void* uwp_GetWindowReference();
extern "C" __declspec(dllimport) int ExternalWinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc);

static HINSTANCE hInst;
static HINSTANCE prev_hInst;

int bootstrap(int argc, char** argv)
{
	std::string cmd = BootMenu::generateParams();

	return ExternalWinMain(hInst, prev_hInst, const_cast<LPSTR>(cmd.c_str()), 0); // TODO: Fix cmd line args
}

int CALLBACK WinMain(HINSTANCE h, HINSTANCE prev, LPSTR argv, int argc)
{
	hInst = h;
	prev_hInst = prev;

	return SDL_WinRTRunApp(bootstrap, NULL);
}
