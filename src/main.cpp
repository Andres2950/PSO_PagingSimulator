#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "imgui.h"
#include "./ImGui/backends/imgui_impl_sdl3.h"
#include "./ImGui/backends/imgui_impl_sdlrenderer3.h"
#include <stdio.h>

static SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    SDL_CreateWindowAndRenderer("Hello World", 1200, 700, 0, &window, &renderer);
    if (window == nullptr || renderer == nullptr) {
        SDL_Log("Couldn't create window or renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_ShowWindow(window);
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();        
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    ImGui_ImplSDL3_ProcessEvent(event);              
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}



// Our state            
bool show_demo_window = true;   
bool otra_window = true;   



/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED){
        SDL_Delay(10); //Sleep if minimize       
    }

    ImGuiIO io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();    

    // === Prepare ImGui windows
    if (show_demo_window){
        ImGui::ShowDemoWindow(&show_demo_window);                               
    }

    //Window
    static float scale = 1.0f; //static hace que sea global
    {
        ImGui::Begin("Otra window");  
        ImVec2 tamano = {400,100};
        ImGui::SetWindowSize("Otra window",tamano);      
        ImGui::SliderFloat("slider float", &scale, 1.0f, 10.0f, "scale = %.2f");        
        ImGui::End();
    }
            
    ImGui::Render(); //finish setting ImGui windows


    // ==== Rendering
    //Clean display buffer
    SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Render SDL Graphics
    int w = 0, h = 0;
    float x, y;
    //const float scale = 4.0f;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, scale, scale);
    const char *message = "SDL funcionando :)";
    x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
    y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, x, y, message);

    //Render ImGui Graphics
    SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
    
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
