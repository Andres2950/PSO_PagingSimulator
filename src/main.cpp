#include "SDL3/SDL_video.h"
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "./ImGui/backends/imgui_impl_sdl3.h"
#include "./ImGui/backends/imgui_impl_sdlrenderer3.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#include "./gui/setup_window.h"

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
  // DEBUG: disable for developtment
  // main_window_flags |= ImGuiWindowFlags_NoMove;
  // main_window_flags |= ImGuiWindowFlags_NoResize;

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
bool show_demo_window = true;
bool setup_window = true;
bool simul_window = false;

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
    showSetupWindow(&setup_window);
  } else {
    simul_window = true;
  }

  if (simul_window) {
    ImGui::SetNextWindowSize(viewportSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::Begin(" ", nullptr, main_window_flags);

    ImGui::PushFont(nullptr, style.FontSizeBase * 2.f);
    ImGui::Text("Simulación");
    int text_height = ImGui::GetItemRectSize().y;
    ImGui::PopFont();
    ImGui::SameLine();
    static bool start = false;
    if (!start && ImGui::Button("Iniciar", ImVec2(0, text_height))) {
      start = true;
    }
    if (start) {
      if (ImGui::Button("Terminar", ImVec2(0, text_height))) {
        start = false;
        simul_window = false;
        setup_window = true;
      }
    }
    ImGui::Separator();
    ImGui::Dummy(ImVec2(viewportSize.x, 1 * scaley)); // padding

    // RAM Section

    style.CellPadding.x = 0;
    if (ImGui::BeginTable("##ram-optimo", 1, ImGuiTableFlags_Borders)) {

      ImGui::TableSetupColumn(" RAM Óptimo");
      ImGui::TableHeadersRow();
      style.CellPadding.y = 0;
      ImGui::TableNextColumn();
      {
        if (ImGui::BeginTable("##ram-optimo-int", 100,
                              ImGuiTableFlags_BordersOuterH |
                                  ImGuiTableFlags_BordersInnerV)) {

          ImGui::TableNextRow(0, 20);
          for (int i = 0; i < 100; i++) {
            ImGui::TableNextColumn();
            if (i < 30) {
              ImU32 cell_bg_color =
                  ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f));
              ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
            }
          }
          ImGui::EndTable();
        }
      }
      style.CellPadding = ImVec2(2, 2);
      ImGui::EndTable();
    }

    style.CellPadding.x = 0;
    if (ImGui::BeginTable("##ram-algoritmo", 1, ImGuiTableFlags_Borders)) {

      ImGui::TableSetupColumn(" RAM Algoritmo");
      ImGui::TableHeadersRow();
      style.CellPadding.y = 0;
      ImGui::TableNextColumn();
      {
        if (ImGui::BeginTable("##ram-algoritmo-int", 100,
                              ImGuiTableFlags_BordersOuterH |
                                  ImGuiTableFlags_BordersInnerV)) {

          ImGui::TableNextRow(0, 20);
          for (int i = 0; i < 100; i++) {
            ImGui::TableNextColumn();
            if (i < 50) {
              ImU32 cell_bg_color =
                  ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f));
              ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
            }
          }
          ImGui::EndTable();
        }
      }
      style.CellPadding = ImVec2(2, 2);
      ImGui::EndTable();
    }

    // MMU Section

    float width_avail = ImGui::GetContentRegionAvail().x;
    if (ImGui::BeginTable("##split", 3, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn(nullptr, 0, width_avail * 0.45);
      ImGui::TableSetupColumn(nullptr, 0, width_avail * 0.1);
      ImGui::TableSetupColumn(nullptr, 0, width_avail * 0.45);

      // MMU Óptimo
      ImGui::TableNextColumn();
      {
        ImGui::SeparatorText("Óptimo");
        if (ImGui::BeginTable(
                "##mmu-tabla-opt", 8,
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |
                    ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
                    ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame,
                ImVec2(0, 270))) {
          ImGui::TableSetupColumn("Page ID");
          ImGui::TableSetupColumn("PID");
          ImGui::TableSetupColumn("Loaded");
          ImGui::TableSetupColumn("L-Addr");
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

        if (ImGui::BeginTable("##detalles-tabla-opt", 3,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("Procesos");
          ImGui::TableSetupColumn("Tiempo de Simulación");
          ImGui::TableSetupColumn("Fragmentación");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::Text("23"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("15s"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("45kB"); // TODO:change for real variable

          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##ram-detalles-opt", 4,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("RAM kB");
          ImGui::TableSetupColumn("RAM %");
          ImGui::TableSetupColumn("V-RAM kB");
          ImGui::TableSetupColumn("V-RAM %");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable

          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##pages-table-opt", 2,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("Páginas Cargadas");
          ImGui::TableSetupColumn("Páginas No Cargadas");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("1%%"); // TODO:change for real variable

          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##thrashing-table-opt", 2,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("Thrashing T");
          ImGui::TableSetupColumn("Thrashing %");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::TableSetBgColor(
              ImGuiTableBgTarget_CellBg,
              ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f)));
          ImGui::Text("12s"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("1%%"); // TODO:change for real variable

          ImGui::EndTable();
        }
      }

      ImGui::TableNextColumn();
      ImGui::TableNextColumn();
      {
        ImGui::SeparatorText("Algoritmo");
        if (ImGui::BeginTable(
                "##mmu-tabla-alg", 8,
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |
                    ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
                    ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame,
                ImVec2(0, 270))) {
          ImGui::TableSetupColumn("Page ID");
          ImGui::TableSetupColumn("PID");
          ImGui::TableSetupColumn("Loaded");
          ImGui::TableSetupColumn("L-Addr");
          ImGui::TableSetupColumn("M-Addr");
          ImGui::TableSetupColumn("D-Addr");
          ImGui::TableSetupColumn("Loaded T");
          ImGui::TableSetupColumn("Mark");
          ImGui::TableHeadersRow();

          ImGui::TableNextColumn();
          for (int i = 0; i < 100; i++) {
            ImGui::TableNextRow(20);
            ImGui::TableNextColumn();
            ImGui::Text("dx");
          }
          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##detalles-tabla-alg", 3,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("Procesos");
          ImGui::TableSetupColumn("Tiempo de Simulación");
          ImGui::TableSetupColumn("Fragmentación");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::Text("23"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("15s"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("45kB"); // TODO:change for real variable

          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##ram-detalles-alg", 4,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("RAM kB");
          ImGui::TableSetupColumn("RAM %");
          ImGui::TableSetupColumn("V-RAM kB");
          ImGui::TableSetupColumn("V-RAM %");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable

          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##pages-table-alg", 2,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("Páginas Cargadas");
          ImGui::TableSetupColumn("Páginas No Cargadas");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::Text("12"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("1%%"); // TODO:change for real variable

          ImGui::EndTable();
        }

        if (ImGui::BeginTable("##thrashing-table-alg", 2,
                              ImGuiTableFlags_Borders)) {
          ImGui::TableSetupColumn("Thrashing T");
          ImGui::TableSetupColumn("Thrashing %");
          ImGui::TableHeadersRow();
          ImGui::TableNextColumn();
          ImGui::TableSetBgColor(
              ImGuiTableBgTarget_CellBg,
              ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f)));
          ImGui::Text("12s"); // TODO:change for real variable
          ImGui::TableNextColumn();
          ImGui::Text("1%%"); // TODO:change for real variable

          ImGui::EndTable();
        }
      }
      ImGui::EndTable();
    }
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
