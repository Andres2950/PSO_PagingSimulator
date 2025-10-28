#include "SDL3/SDL_video.h"
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "./ImGui/backends/imgui_impl_sdl3.h"
#include "./ImGui/backends/imgui_impl_sdlrenderer3.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#include "./gui/setup_window.h"
#include "./gui/simul_window.h"

static SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
float main_scale;
ImGuiWindowFlags main_window_flags;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  /* Create the window */
  main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags =
      SDL_WINDOW_RESIZABLE |
      SDL_WINDOW_HIGH_PIXEL_DENSITY; // | SDL_WINDOW_MAXIMIZED;
  SDL_CreateWindowAndRenderer("Algorimtos de Paginación", 1200 * main_scale,
                              700 * main_scale, window_flags, &window,
                              &renderer);
  if (window == nullptr || renderer == nullptr) {
    SDL_Log("Couldn't create window or renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_SetWindowMinimumSize(window, 854, 480);
  SDL_SetWindowSize(window, 1280, 720);
  SDL_ShowWindow(window);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style

  main_window_flags |= ImGuiWindowFlags_NoSavedSettings;
  main_window_flags |= ImGuiWindowFlags_NoCollapse;
  main_window_flags |= ImGuiWindowFlags_NoMove;
  main_window_flags |= ImGuiWindowFlags_NoResize;
  main_window_flags |= ImGuiWindowFlags_NoTitleBar;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  ImGui_ImplSDL3_ProcessEvent(event);
  if ( // event->type == SDL_EVENT_KEY_DOWN ||
      event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

// Our state
bool setup_window = true;
bool simul_window = false;
int curr_window = 0;
int algorithm = 0;
char *path = nullptr;
int seed = 101;

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
  ImGuiIO io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  style.FontSizeBase = 14;
  style.WindowPadding = ImVec2(30, 10);
  style.ItemSpacing = ImVec2(10, 12);
  style.FontScaleMain = 1;
  style.CellPadding = ImVec2(2, 2);

  if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
    SDL_Delay(10); // Sleep if minimize
  }

  // Start the Dear ImGui frame
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  if (path == nullptr) {
    path = (char *)calloc(sizeof(char), 4096);
  }

  // Windows
  if (curr_window == 0) {
    showSetupWindow(&setup_window, &algorithm, &seed, path, main_window_flags);
  } else {
    showSimulWindow(&simul_window, algorithm, seed, path, main_window_flags);
  }

  if (!setup_window && curr_window == 0) {
    simul_window = true;
    curr_window = 1;
  }

  if (!simul_window && curr_window == 1) {
    setup_window = true;
    curr_window = 0;
  }

  ImGui::Render(); // finish setting ImGui windows

  // ==== Rendering
  // Clean display buffer
  SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Render SDL Graphics
  int w = 0, h = 0;
  // const float scale = 4.0f;
  SDL_GetRenderOutputSize(renderer, &w, &h);
  // SDL_SetRenderScale(renderer, scale, scale);

  // Render ImGui Graphics
  SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
