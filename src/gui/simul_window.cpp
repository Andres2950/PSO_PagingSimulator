#include "../computer_refactor/Parser.h"
#include "../constants.h"
#include "SDL3/SDL_timer.h"
#include "imgui.h"

// State
Parser *parser;
int delay = 0;
uint last_execution = SDL_GetTicks();

void showSimulWindow(bool *open, int algorithm, const char *filepath,
                     ImGuiWindowFlags windowFlags = 0) {

  // State Init
  if (parser == nullptr) {
    parser = new Parser(algorithm, filepath);
  }

  //  Common Variables
  ImGuiStyle style = ImGui::GetStyle();
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

  ImGui::SetNextWindowSize(viewportSize);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
  ImGui::Begin(" ", nullptr, windowFlags);

  ImGui::Dummy(ImVec2(viewportSize.x, 0)); // padding
  ImGui::PushFont(nullptr, style.FontSizeBase * 2.f);
  ImGui::Text("Simulación");
  int text_height = ImGui::GetItemRectSize().y;
  ImGui::PopFont();
  ImGui::SameLine();
  static bool start = false;
  if (!start) {
    if (ImGui::Button("Iniciar", ImVec2(0, text_height))) {
      start = true;
      parser = new Parser(algorithm, filepath);
    }
    ImGui::SameLine();
    if (ImGui::Button("Volver", ImVec2(0, text_height))) {
      *open = false;
      start = false;
    }
  }

  if (start) {
    if (ImGui::Button("Terminar", ImVec2(0, text_height))) {
      parser = new Parser(algorithm, filepath);
      start = false;
    }
  }
  ImGui::SameLine(0, 100);
  ImGui::Text("Delay (ms):");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 100);
  ImGui::SliderInt("##delay-slider", &delay, 0, 5000);
  ImGui::Separator();
  ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding

  // Delay controller
  if (start) {
    if (SDL_GetTicks() - last_execution > delay) {
      last_execution = SDL_GetTicks();
      start = parser->executeInstruction();
    }
  }

  // RAM Section

  style.CellPadding.x = 0;
  if (ImGui::BeginTable("##ram-optimo", 1, ImGuiTableFlags_Borders)) {

    ImGui::TableSetupColumn(" RAM Óptimo");
    ImGui::TableHeadersRow();
    style.CellPadding.y = 0;
    ImGui::TableNextColumn();
    {
      if (ImGui::BeginTable("##ram-optimo-int", 100,
                            ImGuiTableFlags_BordersInnerV)) {

        ImGui::TableNextRow(0, 20);
        for (int i = 0; i < MEMORY_SIZE; i++) {
          ImGui::TableNextColumn();
          if (parser->optimal_mmu->memory[i] != -1) {
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
                            ImGuiTableFlags_BordersInnerV)) {

        ImGui::TableNextRow(0, 20);
        for (int i = 0; i < 100; i++) {
          ImGui::TableNextColumn();
          if (parser->other_mmu->memory[i] != -1) {
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
      auto mmu = parser->optimal_mmu;
      auto disk = mmu->disk;
      int loaded_pages = 0;
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
        for (auto it = disk.begin(); it != disk.end(); it++) {
          ImGui::TableNextRow(0, 20);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.id);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.pid);
          ImGui::TableNextColumn();
          const char *str = it->second.is_loaded ? " X " : "  ";
          loaded_pages = it->second.is_loaded ? loaded_pages + 1 : loaded_pages;
          ImGui::Text("%s", str);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.l_addr);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.m_addr);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.d_addr);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.load_t);
          ImGui::TableNextColumn();
          ImGui::Text("%d", it->second.mark);
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
        ImGui::Text("%ds", mmu->time);
        ImGui::TableNextColumn();
        ImGui::Text("45kB"); // TODO:change for real variable

        ImGui::EndTable();
      }

      if (ImGui::BeginTable("##ram-detalles-opt", 4, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("RAM kB");
        ImGui::TableSetupColumn("RAM %");
        ImGui::TableSetupColumn("V-RAM kB");
        ImGui::TableSetupColumn("V-RAM %");
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::Text("%d", loaded_pages * PAGE_SIZE / 1000);
        ImGui::TableNextColumn();
        ImGui::Text("%0.f", (float)loaded_pages / MEMORY_SIZE * 100);
        ImGui::TableNextColumn();
        ImGui::Text("%ld", disk.size() * PAGE_SIZE / 1000);
        ImGui::TableNextColumn();
        ImGui::Text("%0.f", (float)disk.size() / MEMORY_SIZE * 100);

        ImGui::EndTable();
      }

      if (ImGui::BeginTable("##pages-table-opt", 2, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Páginas Cargadas");
        ImGui::TableSetupColumn("Páginas No Cargadas");
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::Text("%d", loaded_pages);
        ImGui::TableNextColumn();
        ImGui::Text("%ld", disk.size() - loaded_pages);

        ImGui::EndTable();
      }

      if (ImGui::BeginTable("##thrashing-table-opt", 2,
                            ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Thrashing T");
        ImGui::TableSetupColumn("Thrashing %");
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                               ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f)));
        ImGui::Text("%d", mmu->fault_time);
        ImGui::TableNextColumn();
        float thrasing_percentage =
            mmu->time != 0 ? (float)mmu->fault_time / mmu->time * 100 : 0;
        ImGui::Text("%0.f", thrasing_percentage);

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
          ImGui::TableNextRow(0, 20);
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

      if (ImGui::BeginTable("##ram-detalles-alg", 4, ImGuiTableFlags_Borders)) {
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

      if (ImGui::BeginTable("##pages-table-alg", 2, ImGuiTableFlags_Borders)) {
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
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
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
