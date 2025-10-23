cmake_minimum_required(VERSION 3.16)

project(so-proyecto2)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app/libs$<CONFIGURATION>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

# This makes rpath relative to the binary so its independent of the system and the directory
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

# This assumes the SDL source is available in src/SDL
add_subdirectory(src/SDL EXCLUDE_FROM_ALL)

#Static link SDL to prevent missing libraries in dest OS
#Leave turned off because the program will be compiled in the dest OS
#when OFF there is no need to install more libraries
#set(SDL_STATIC OFF)

# Set ImGui sources
set(SOURCES ./src/ImGui/backends/imgui_impl_sdlrenderer3.cpp ./src/ImGui/backends/imgui_impl_sdl3.cpp ./src/ImGui/imgui.cpp ./src/ImGui/imgui_demo.cpp ./src/ImGui/imgui_draw.cpp ./src/ImGui/imgui_tables.cpp ./src/ImGui/imgui_widgets.cpp )


set(SOURCES ${SOURCES} ./src/gui/setup_window.h )

# Create executable target
add_executable(app ./src/main.cpp ${SOURCES})

# Link to the actual SDL3 library.
target_link_libraries(app PUBLIC SDL3::SDL3)

# Link headers
target_include_directories(app PUBLIC  ./src/ImGui/backends ./src/ImGui ./src/gui)
