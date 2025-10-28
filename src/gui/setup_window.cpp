#include "../computer/utils.h"
#include "../constants.h"
#include "SDL3/SDL_video.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <cstring>

// State
int procesos = 5;
int operaciones = 10;

// File handling funcitons

static const SDL_DialogFileFilter filters[] = {
    {"Instruction List", "instr"}, {"All", "*"} // DEBUG
};

void openFile(void *userdata, const char *const *filelist, int filter) {
  if (filelist != nullptr && filelist[0] != nullptr) {
    char *path = (char *)userdata;
    strncpy(path, filelist[0], 4096);
  }
}

void saveFile(void *userdata, const char *const *filelist, int filter) {
  if (filelist != nullptr && filelist[0] != nullptr) {
    int semilla = *(int *)userdata;
    create_operations(procesos, operaciones, filelist[0], semilla);
  }
}

// Window

void showSetupWindow(bool *open, int *algorithm, int *semilla, char *path,
                     ImGuiWindowFlags window_flags = 0) {

  // Restrictions
  if (operaciones < procesos) {
    operaciones = procesos;
  }
  if (*semilla < 0) {
    *semilla = 0;
  }
  if (procesos < 1) {
    procesos = 1;
  }

  // Common Variables
  static SDL_Window *window = SDL_GL_GetCurrentWindow();
  ImGuiStyle style = ImGui::GetStyle();
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));

  // Window
  ImGui::SetNextWindowSize(viewportSize);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
  ImGui::Begin(" ", nullptr, window_flags);

  ImGui::Dummy(ImVec2(viewportSize.x, 0)); // padding
  ImGui::PushFont(nullptr, style.FontSizeBase * 2.f);
  ImGui::Text("Configuración");
  ImGui::Separator();
  ImGui::PopFont();
  ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding

  ImGui::Indent(10);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Semilla: ");
  ImGui::SameLine(150);
  ImGui::SetNextItemWidth(300);
  ImGui::InputInt("##semilla", semilla);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Algoritmo: ");
  ImGui::SameLine(150);
  ImGui::RadioButton("FIFO", algorithm, ALG_FIFO);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("Second Chance", algorithm, ALG_SECOND_CHANCE);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("MRU", algorithm, ALG_MRU);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("LRU", algorithm, ALG_LRU);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("Random", algorithm, ALG_RANDOM);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Archivo de Instrucciones: ");
  ImGui::SameLine(0, 10);
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.9f - 200);
  ImGui::InputText("##input1", path, 4096, ImGuiInputTextFlags_ReadOnly);
  ImGui::SameLine(0, 5);
  bool load_btn_pressed = ImGui::Button("Cargar", ImVec2(100, 20));
  if (load_btn_pressed) {
    SDL_ShowOpenFileDialog(openFile, path, window, filters, 2, nullptr,
                           false); // DEBUG:poner 1 en realese
  }
  ImGui::SameLine(0, 5);
  bool generate_btn_pressed = ImGui::Button("Generar", ImVec2(100, 20));
  if (generate_btn_pressed) {
    ImGui::OpenPopup("Generar Lista de Instrucciones");
  }

  ImGui::PopStyleVar();
  ImGui::SetNextWindowSize(ImVec2(300, 150));
  ImGui::SetNextWindowPos(
      ImVec2(viewportSize.x / 2 - 150, viewportSize.y / 2 - 75),
      ImGuiCond_Once);
  if (ImGui::BeginPopupModal("Generar Lista de Instrucciones", nullptr,
                             ImGuiWindowFlags_NoResize)) {
    ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Procesos: ");
    ImGui::SameLine(150);
    ImGui::SetNextItemWidth(120);
    ImGui::InputInt("##procesos", &procesos);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Operaciones: ");
    ImGui::SameLine(150);
    ImGui::SetNextItemWidth(120);
    ImGui::InputInt("##operaciones", &operaciones);

    ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding

    bool generate_pop_btn_pressed = ImGui::Button("Generar", ImVec2(100, 20));
    if (generate_pop_btn_pressed) {
      SDL_ShowSaveFileDialog(saveFile, semilla, window, filters, 2, nullptr);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine(0, 10);
    bool cancel_pop_btn_pressed = ImGui::Button("Cancelar", ImVec2(100, 20));
    if (cancel_pop_btn_pressed) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));

  ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding
  if (ImGui::Button("Iniciar Simulación", ImVec2(180, 20))) {
    if (strlen(path) == 0) {
      ImGui::OpenPopup("##warning-popup");
    } else {
      *open = false;
    }
  }

  ImGui::SetNextWindowSize(ImVec2(300, 75));
  ImGui::SetNextWindowPos(
      ImVec2(viewportSize.x / 2 - 150, viewportSize.y / 2 - 40),
      ImGuiCond_Always);
  if (ImGui::BeginPopup("##warning-popup")) {
    ImGui::SetNextItemWidth(300);
    ImGui::TextWrapped(
        ("Error: Debe escoger un archivo de instrucciones para iniciar "
         "la simulación"));
    ImGui::EndPopup();
  }

  ImGui::Unindent(10);

  ImGui::End();

  ImGui::PopStyleVar();
}
