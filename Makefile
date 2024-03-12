IMGUI_SRC := $(wildcard include/imgui/*.cpp)
THREEDL_SRC := $(wildcard src/engine/*.cpp)
THREEDL_SRC += $(wildcard src/engine/GPU/*.cpp)

make:
	g++ -std=c++20 -o 3DL.app src/main.cpp $(IMGUI_SRC) $(THREEDL_SRC) -lSDL2 -lm -Wall -O3 -ffast-math -lSDL2_image -lSDL2_ttf -lOpenCL -Iinclude/imgui -Iinclude/SDL2