#include "SDL3/SDL_dialog.h"
#include "SDL3/SDL_video.h"
#include <cstddef>
#include <cstring>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "./ImGui/backends/imgui_impl_sdl3.h"
#include "./ImGui/backends/imgui_impl_sdlrenderer3.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>

// File Handling

char *path;

static const SDL_DialogFileFilter filters[] = {
    {"Instruction List", "instr"}, {"All", "*"} // DEBUG
};

void openFile(void *userdata, const char *const *filelist, int filter) {
  if (filelist != nullptr && filelist[0] != nullptr) {
    strncpy(path, filelist[0], 4096);
    // TODO: conectar esto
  }
}

void saveFile(void *userdata, const char *const *filelist, int filter) {
  if (filelist != nullptr && filelist[0] != nullptr) {
    printf("%s", filelist[0]);
    // TODO:conectar esto
  }
}

static SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
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
  // DEBUG: disable for developtment
  // main_window_flags |= ImGuiWindowFlags_NoMove;
  // main_window_flags |= ImGuiWindowFlags_NoResize;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  // Initialize vars
  path = (char *)calloc(sizeof(char), 4096);
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
bool show_demo_window = true;
bool setup_window = false;
bool simul_window = true;

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
  float scalex = 1; // viewportSize.x/640;
  float scaley = 1; // viewportSize.x/640;
  ImGuiIO io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  style.FontSizeBase = 14;
  style.WindowPadding = ImVec2(30 * scalex, 10 * scaley);
  style.ItemSpacing = ImVec2(10 * scalex, 12 * scaley);
  style.FontScaleMain = scaley;
  style.CellPadding = ImVec2(2, 2);

  if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
    SDL_Delay(10); // Sleep if minimize
  }

  // Start the Dear ImGui frame
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  // ==== Prepare ImGui windows
  if (show_demo_window) {
    ImGui::ShowDemoWindow(&show_demo_window);
  }

  // Window
  if (setup_window) {
    ImGui::SetNextWindowSize(viewportSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::Begin(" ", nullptr, main_window_flags);

    // ImGui::PushFont(nullptr, style.FontSizeBase * 2.f);
    // ImGui::Text("Simulador de Algoritmos de Paginación");
    // ImGui::PopFont();

    // ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding
    ImGui::PushFont(nullptr, style.FontSizeBase * 1.5f);
    ImGui::SeparatorText("Configuración");
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding

    ImGui::Indent(10 * scalex);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Semilla: ");
    ImGui::SameLine(150 * scalex);
    ImGui::SetNextItemWidth(300 * scalex);
    static int semilla = 911;
    ImGui::InputInt("##semilla", &semilla);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Algoritmo: ");
    ImGui::SameLine(150 * scalex);
    static int algoritmo = 0;
    ImGui::RadioButton("FIFO", &algoritmo, 0);
    ImGui::SameLine(0, 20 * scalex);
    ImGui::RadioButton("Second Chance", &algoritmo, 1);
    ImGui::SameLine(0, 20 * scalex);
    ImGui::RadioButton("MRU", &algoritmo, 2);
    ImGui::SameLine(0, 20 * scalex);
    ImGui::RadioButton("Random", &algoritmo, 3);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Archivo de Instrucciones: ");
    ImGui::SameLine(0, 10 * scalex);
    ImGui::SetNextItemWidth(330);
    ImGui::InputText("##input1", path, 4096, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine(0, 5 * scalex);
    bool load_btn_pressed =
        ImGui::Button("Cargar", ImVec2(100 * scalex, 20 * scaley));
    if (load_btn_pressed) {
      SDL_ShowOpenFileDialog(openFile, nullptr, window, filters, 2, nullptr,
                             false); // DEBUG:poner 1 en realese
    }
    ImGui::SameLine(0, 5 * scalex);
    bool generate_btn_pressed =
        ImGui::Button("Generar", ImVec2(100 * scalex, 20 * scaley));
    if (generate_btn_pressed) {
      ImGui::OpenPopup("Generar Lista de Instrucciones");
    }

    ImGui::SetNextWindowSize(ImVec2(280, 150));
    if (ImGui::BeginPopupModal("Generar Lista de Instrucciones")) {
      ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Procesos: ");
      ImGui::SameLine(150 * scalex);
      ImGui::SetNextItemWidth(100 * scalex);
      static int procesos = 101;
      ImGui::InputInt("##procesos", &procesos);

      ImGui::AlignTextToFramePadding();
      ImGui::Text("Operaciones: ");
      ImGui::SameLine(150 * scalex);
      ImGui::SetNextItemWidth(100 * scalex);
      static int operaciones = 1000;
      ImGui::InputInt("##operaciones", &operaciones);

      ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding

      bool generate_pop_btn_pressed =
          ImGui::Button("Generar", ImVec2(100 * scalex, 20 * scaley));
      if (generate_pop_btn_pressed) {
        SDL_ShowSaveFileDialog(saveFile, nullptr, window, filters, 2, nullptr);
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine(0, 10 * scalex);
      bool cancel_pop_btn_pressed =
          ImGui::Button("Cancelar", ImVec2(100 * scalex, 20 * scaley));
      if (cancel_pop_btn_pressed) {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding
    bool simul_btn_pressed =
        ImGui::Button("Iniciar Simulación", ImVec2(180 * scalex, 20 * scaley));
    if (simul_btn_pressed) {
      setup_window = false;
      simul_window = true;
      // TODO: conectar este boton
    }

    ImGui::Unindent(10 * scalex);

    ImGui::End();
  }

  if (simul_window) {
    ImGui::SetNextWindowSize(viewportSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::Begin(" ", nullptr, main_window_flags);

    ImGui::PushFont(nullptr, style.FontSizeBase * 1.5f);
    ImGui::Text("Simulación");
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding

    ImGui::Indent(10 * scalex);

    // RAM Section
    style.CellPadding.x = 0;
    ImGui::BeginTable("##ram-optimo", 1, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn(" RAM Óptimo");
    ImGui::TableHeadersRow();
    style.CellPadding.y = 0;
    ImGui::TableNextColumn();
    {
      ImGui::BeginTable("##ram-optimo-int", 100,
                        ImGuiTableFlags_BordersOuterH |
                            ImGuiTableFlags_BordersInnerV);
      ImGui::TableNextRow(0, 20);
      for (int i = 0; i < 100; i++) {
        ImGui::TableNextColumn();
        if (i < 30) {
          ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f));
          ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
        }
      }
      ImGui::EndTable();
    }
    style.CellPadding = ImVec2(2, 2);
    ImGui::EndTable();

    ImGui::BeginTable("##ram-algoritmo", 1, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn(" RAM Algoritmo");
    ImGui::TableHeadersRow();
    ImGui::TableNextColumn();
    {
      ImGui::BeginTable("##ram-algoritmo-int", 100, ImGuiTableFlags_Borders);
      ImGui::TableNextRow(0, 20);
      for (int i = 0; i < 100; i++) {
        ImGui::TableNextColumn();
      }
      ImGui::EndTable();
    }
    ImGui::EndTable();

    ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding

    // MMU Section

    float width_avail = ImGui::GetContentRegionAvail().x;
    ImGui::BeginTable("##split", 3, ImGuiTableFlags_SizingFixedFit);
    ImGui::TableSetupColumn(nullptr, 0, width_avail * 0.45);
    ImGui::TableSetupColumn(nullptr, 0, width_avail * 0.1);
    ImGui::TableSetupColumn(nullptr, 0, width_avail * 0.45);

    ImGui::TableNextColumn();
    {
      ImGui::SeparatorText("Óptimo");
      ImGui::BeginTable("##mmu-optimo", 8,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                            ImGuiTableFlags_ScrollY,
                        ImVec2(0, 270));

      ImGui::TableSetupColumn("Page ID");
      ImGui::TableSetupColumn("PID");
      ImGui::TableSetupColumn("Loaded");
      ImGui::TableSetupColumn("M-Addr");
      ImGui::TableSetupColumn("M-Addr");
      ImGui::TableSetupColumn("D-Addr");
      ImGui::TableSetupColumn("Loaded T");
      ImGui::TableSetupColumn("Mark");
      ImGui::TableHeadersRow();

      ImGui::TableNextColumn();
      for (int i = 0; i < 100; i++) {
        ImGui::TableNextRow(20);
        ImGui::TableNextColumn();
        ImGui::Text("xd");
      }
      ImGui::EndTable();
    }

    ImGui::TableNextColumn();
    ImGui::TableNextColumn();
    {
      ImGui::SeparatorText("Algoritmo");
      ImGui::Text("duplicado");
    }

    ImGui::EndTable();

    ImGui::Unindent(10 * scalex);
    ImGui::End();
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
