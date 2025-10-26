#ifndef SETUP_WINDOW
#define SETUP_WINDOW

#include "imgui.h"
#include <SDL3/SDL.h>

void openFile(void *userdata, const char *const *filelist, int filter);

void saveFile(void *userdata, const char *const *filelist, int filter);

// Window

void showSetupWindow(bool *open, int *algorithm, char *path,
                     ImGuiWindowFlags window_flags = 0);

#endif
