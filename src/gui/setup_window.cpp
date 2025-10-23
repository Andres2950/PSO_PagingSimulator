#include "SDL3/SDL_video.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <stdio.h>

// State

char *path;

// File handling funcitons

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

// Window

void showSetupWindow(bool *open, ImGuiWindowFlags window_flags = 0) {
  // State
  if (path == nullptr) {
    path = (char *)calloc(sizeof(char), 4096);
  }

  static int semilla = 911;
  static int algoritmo = 0;
  static int procesos = 101;
  static int operaciones = 1000;

  // Common Variables
  static SDL_Window *window = SDL_GL_GetCurrentWindow();
  ImGuiStyle style = ImGui::GetStyle();
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

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
  ImGui::InputInt("##semilla", &semilla);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Algoritmo: ");
  ImGui::SameLine(150);
  ImGui::RadioButton("FIFO", &algoritmo, 0);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("Second Chance", &algoritmo, 1);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("MRU", &algoritmo, 2);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("LRU", &algoritmo, 3);
  ImGui::SameLine(0, 20);
  ImGui::RadioButton("Random", &algoritmo, 4);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Archivo de Instrucciones: ");
  ImGui::SameLine(0, 10);
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.9f - 200);
  ImGui::InputText("##input1", path, 4096, ImGuiInputTextFlags_ReadOnly);
  ImGui::SameLine(0, 5);
  bool load_btn_pressed = ImGui::Button("Cargar", ImVec2(100, 20));
  if (load_btn_pressed) {
    SDL_ShowOpenFileDialog(openFile, nullptr, window, filters, 2, nullptr,
                           false); // DEBUG:poner 1 en realese
  }
  ImGui::SameLine(0, 5);
  bool generate_btn_pressed = ImGui::Button("Generar", ImVec2(100, 20));
  if (generate_btn_pressed) {
    ImGui::OpenPopup("Generar Lista de Instrucciones");
  }

  ImGui::SetNextWindowSize(ImVec2(280, 150));
  if (ImGui::BeginPopupModal("Generar Lista de Instrucciones")) {
    ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Procesos: ");
    ImGui::SameLine(150);
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##procesos", &procesos);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Operaciones: ");
    ImGui::SameLine(150);
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##operaciones", &operaciones);

    ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding

    bool generate_pop_btn_pressed = ImGui::Button("Generar", ImVec2(100, 20));
    if (generate_pop_btn_pressed) {
      SDL_ShowSaveFileDialog(saveFile, nullptr, window, filters, 2, nullptr);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine(0, 10);
    bool cancel_pop_btn_pressed = ImGui::Button("Cancelar", ImVec2(100, 20));
    if (cancel_pop_btn_pressed) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding
  if (ImGui::Button("Iniciar Simulación", ImVec2(180, 20))) {
    *open = false;
    // TODO: conectar este boton
  }

  ImGui::Unindent(10);

  ImGui::End();
}
