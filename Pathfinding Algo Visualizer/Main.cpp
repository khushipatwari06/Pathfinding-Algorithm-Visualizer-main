#include "Algos.h"
#include "Window.h"
int main(int argc, char **argv) {
	Window* window = new Window("Pathfinding Algorithm Visualizer", 930, 630);
	TTF_Init();
	while (!window->is_Closed()) {
		window->poll_Events();
		if (window->input != false) {
			window->update();
			window->clear();
		}
		
	}
	window->~Window();
	delete window;
	window = nullptr;
	return 0;
}
