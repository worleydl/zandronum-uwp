#include "bootmenu.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <string>
#include <vector>

#include "wad.h"

using namespace std;

static string selected = "";
static vector<string> enabledMods;

static bool ready = false;

void menuPickWad(vector<string> wads, vector<string> mods)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    static std::vector<bool> enabled(mods.size(), false);  // On/off state for each item

    ImGui::NewLine();
    ImGui::Text("Select IWAD:");

    ImGui::BeginListBox("IWAD List", ImVec2(windowSize.x, windowSize.y / 3.0));
    for (auto entry : wads) {
        if (ImGui::Selectable(entry.c_str(), false)) {
            selected = entry;
            for (int i = 0; i < enabled.size(); i++) {
                if (enabled[i])
                    enabledMods.push_back(mods[i]);
            }
            ready = true;
        }
    }

    ImGui::EndListBox();
    ImGui::NewLine();

    ImGui::Text("Enable/Disable mods:");
    ImGui::BeginListBox("ToggleList", ImVec2(windowSize.x, windowSize.y / 3.0));
    for (size_t i = 0; i < mods.size(); i++)
    {
        // Use ImGui::Selectable with a toggle effect
        if (ImGui::Selectable(mods[i].c_str(), enabled[i]))
            enabled[i] = !enabled[i];
    }
    ImGui::EndListBox();


}

void menuNoWad()
{
    const char* NO_WADS = "No wads were found, try installing some.";
    const char* SUB_MSG= "Install wads under `E:/zandronum` or next to executable.";

    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 text_size = ImGui::CalcTextSize(NO_WADS);

    float x = (window_size.x - text_size.x) * 0.5f;
    float y = (window_size.y - text_size.y) * 0.5f;

    ImGui::SetCursorPos(ImVec2(x, y));
    ImGui::Text(NO_WADS);

    text_size = ImGui::CalcTextSize(SUB_MSG);
    x = (window_size.x - text_size.x) * 0.5f;
    ImGui::SetCursorPosX(x);
    ImGui::Text("Install wads under `E:/zandronum` or inside program folder.");
}

std::string BootMenu::generateParams()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Rect rect;
    SDL_GetDisplayBounds(0, &rect);

    SDL_Window* window = SDL_CreateWindow("Zandronum Preflight", 0, 0,
        1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader (no need for glad if already included via SDL)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.FontGlobalScale = rect.h / 720.0f;

    ImGui::StyleColorsDark();

    // Init backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

	std::vector<std::string> found_wads = WAD::find_wads();
	std::vector<std::string> found_mods = WAD::find_aux();

    bool wadsAvailable = found_wads.size() > 0;

    // Main loop
    while (!ready) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                ready = true;
        }

        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Make window take up entire screen, no styles
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("Zandronum Boot Config", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
        ImGui::Text("Zandronum UWP Edition");


        if (wadsAvailable) {
            menuPickWad(found_wads, found_mods);
        }
        else {
            menuNoWad();
        }

        ImGui::End();

        // Render
        ImGui::PopStyleVar(1);
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);

    std::string cmdline = "zandronum-uwp.exe -iwad " + selected;
    for (auto entry : enabledMods) {
        cmdline += " -file " + entry;
    }

    return cmdline;
}
