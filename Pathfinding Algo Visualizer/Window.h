#pragma once
#include <SDL.h>
#include "Algos.h"
#include <SDL_ttf.h>
class Window {
friend class Graph;
public:
	Window(const std::string& title, int width, int height);
	~Window();
    bool init_SDL(const std::string& title, int width, int height);
	void poll_Events();
	void clear(); //Draw Function
    void update();	// find path
	inline bool is_Closed() const { return closed; }
	int algos = 3;
	bool input;
private:
	bool algos_Pressed = false;
	bool heuristic_Pressed = false;
	std::string title;
	int width = 670;
	int height = 670;
	int node_Size = 30;
	int node_Gap = 40;
	bool closed = false;
    
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
    Graph graph;
};


