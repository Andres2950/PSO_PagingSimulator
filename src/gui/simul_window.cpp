#include "imgui.h"

void showSimulWindow(bool *open, ImGuiWindowFlags windowFlags = 0) {
  // State

  //  Common Variables
  ImGuiStyle style = ImGui::GetStyle();
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

  ImGui::SetNextWindowSize(viewportSize);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
  ImGui::Begin(" ", nullptr, windowFlags);

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
      *open = false;
      start = false;
    }
  }
  ImGui::Separator();
  ImGui::Dummy(ImVec2(viewportSize.x, 1)); // padding

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

      if (ImGui::BeginTable("##ram-detalles-opt", 4, ImGuiTableFlags_Borders)) {
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

      if (ImGui::BeginTable("##pages-table-opt", 2, ImGuiTableFlags_Borders)) {
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
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
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
