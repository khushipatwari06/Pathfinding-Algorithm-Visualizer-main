#include "Window.h"

Window::Window(const std::string& title, int width, int height) {
	closed = !init_SDL( title, width, height);
	input = true;
}

Window::~Window(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
}

bool Window::init_SDL(const std::string& title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Failed to initialize SDL. \n";
		return 0;
	}
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf. \n";
		return 0;
	}
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (window == nullptr){
		std::cerr << "Failed to create window. \n";
			return 0;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr) {
		std::cerr << "Failed to create renderer. \n ";
		return 0;
	}
	return true;
};

void Window::poll_Events() {
	SDL_Event event;
	if (SDL_PollEvent(&event) && !closed) {
		switch (event.type) {
		case SDL_QUIT:
			closed = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				int node_ClickedX = event.motion.x / node_Gap;
				int node_ClickedY = event.motion.y / node_Gap;
				if (node_ClickedX >= 0 && node_ClickedX <= graph.graph_Width) {
					if (node_ClickedY >= 0 && node_ClickedY <= graph.graph_Height) {
						const Uint8* key_Input = SDL_GetKeyboardState(NULL);
						if (key_Input[SDL_SCANCODE_LSHIFT]) {
							graph.set_Start(node_ClickedX, node_ClickedY);
						}
						else if (key_Input[SDL_SCANCODE_LCTRL]){
							graph.set_End(node_ClickedX, node_ClickedY);
						}
						else if (key_Input[SDL_SCANCODE_W]) {
							graph.switch_Wall(node_ClickedX, node_ClickedY);
						}
						else if (key_Input[SDL_SCANCODE_Q] && algos != 1) {
							graph.switch_Weight(node_ClickedX, node_ClickedY);
						}
						input = true;
					}
				}
				if (event.motion.x >= 660 && event.motion.x <= 900 && event.motion.y >= 30 && event.motion.y <= 130 && !algos_Pressed && !heuristic_Pressed) {
					algos_Pressed = true;
					input = true;
					break;
				}
				else if (event.motion.x >= 660 && event.motion.x <= 900 && event.motion.y >= 160 && event.motion.y <= 260 && !algos_Pressed && !heuristic_Pressed) {
					heuristic_Pressed = true;
					input = true;
					break;
				}
				if (algos_Pressed) {
					for (int i{ 0 }; i < 3; ++i) {
						if (event.motion.x >= 660 && event.motion.x <= 900 && event.motion.y >= 30 + i*130 && event.motion.y <= 130 + i*130) {
							algos = i;
							algos_Pressed = false;
							input = true;
							break;
						}
					}
				}
				if (heuristic_Pressed) {
					for (int i{ 0 }; i < 3; ++i) {
						if (event.motion.x >= 660 && event.motion.x <= 900 && event.motion.y >= 30 + i * 130 && event.motion.y <= 130 + i * 130) {
							graph.h = i;
							heuristic_Pressed = false;
							input = true;
							break;
						}
					}
				}
			}
			break;
		}		
	}
}

void Window::update() {
	if (input) {
		if (algos == 0) {
			graph.Astar();
		}
		else if (algos == 1) {
			graph.BFS();
		}
		else if (algos == 2) {
			graph.Djikstra();
		}
		input = false;
	}
}

void Window::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Setting colour to black for background
	SDL_RenderClear(renderer);
	Graph::Node* nodes = graph.get_Nodes();
	//Drawing Node connections (the lines from one Node to another)
	SDL_SetRenderDrawColor(renderer, 102, 153, 255, 255);
	for (int i{ 0 }; i < graph.graph_Width; ++i) {
		//Drawing Vertical Lines
		int y1 = 0;
		int y2 = (graph.graph_Height - 1) * node_Gap;
		int x1 = i * node_Gap + node_Size * 0.5;
		SDL_RenderDrawLine(renderer, x1, y1, x1, y2);
		//Drawing Horizontal Lines
		SDL_RenderDrawLine(renderer, y1, x1, y2, x1);
	}

	//Drawing Nodes on top of connections to represent graph
	Graph::Node* sn = graph.get_Start();
	Graph::Node* en = graph.get_End();

	//Drawing Buttons
	std::string file_Path = "arial.ttf";
	TTF_Font* font = TTF_OpenFont(file_Path.c_str(), 30);
	if (font == NULL) {
		throw "Error!";
	}
	SDL_SetRenderDrawColor(renderer, 217, 217, 217, 255);
	SDL_Rect msg_Rect;
	msg_Rect.w = 220;
	msg_Rect.h = 80;
	msg_Rect.x = 670;
	SDL_Rect button;
	button.w = 240;
	button.h = 100;
	button.x = 660;
	SDL_Color black = { 0,0,0 };
	for (int i{ 0 }; i < 2; ++i) {
		if (i == 1 && algos == 0) {
			SDL_SetRenderDrawColor(renderer, 217, 217, 217, 255);
			button.y = 30 + i * 130;
			SDL_RenderFillRect(renderer, &button);
		}
		else if (i == 0) {
			button.y = 30 + i * 130;
			SDL_RenderFillRect(renderer, &button);
		}
		msg_Rect.y = 40 + i * 130;
		if (i == 0) {
			SDL_Surface* msg = TTF_RenderText_Solid(font, "Algorithms", black);
			SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
			SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
		}
		else if (i == 1 && algos == 0) {
			SDL_Surface* msg = TTF_RenderText_Solid(font, "Heuristics", black);
			SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
			SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
		}
		//Algorithm option buttons
		SDL_SetRenderDrawColor(renderer, 217, 217, 217, 255);
		if (algos_Pressed) {
			for (int i{ 0 }; i < 3; ++i) {
				button.y = 30 + i * 130;
				SDL_RenderFillRect(renderer, &button);
				msg_Rect.y = 40 + i * 130;
				if (i == 0) {
					SDL_Surface* msg = TTF_RenderText_Solid(font, "A Star", { 0,0,0 });
					SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
					SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
				}
				else if (i == 1) {
					SDL_Surface* msg = TTF_RenderText_Solid(font, "Breadth First Search", { 0,0,0 });
					SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
					SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
				}
				else {
					SDL_Surface* msg = TTF_RenderText_Solid(font, "Djikstra's", { 0,0,0 });
					SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
					SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
				}
			}
		}

		if (heuristic_Pressed) {
			for (int i{ 0 }; i < 3; ++i) {
				button.y = 30 + i * 130;
				SDL_RenderFillRect(renderer, &button);
				msg_Rect.y = 40 + i * 130;
				if (i == 0) {
					SDL_Surface* msg = TTF_RenderText_Solid(font, "Manhattan Distance", { 0,0,0 });
					SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
					SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
				}
				else if (i == 1) {
					SDL_Surface* msg = TTF_RenderText_Solid(font, "Diagonal Distance", { 0,0,0 });
					SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
					SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
				}
				else {
					SDL_Surface* msg = TTF_RenderText_Solid(font, "Euclidean Distance", { 0,0,0 });
					SDL_Texture* msg_Texture = SDL_CreateTextureFromSurface(renderer, msg);
					SDL_RenderCopy(renderer, msg_Texture, nullptr, &msg_Rect);
				}
			}
		}
		
		for (int i{ 0 }; i < graph.graph_Width; ++i) {
			for (int j{ 0 }; j < graph.graph_Height; ++j) {
				SDL_Rect v_Node;
				v_Node.w = node_Size;
				v_Node.h = node_Size;
				v_Node.x = i * node_Gap;
				v_Node.y = j * node_Gap;
				if (nodes[j * graph.graph_Height + i].wall) {
					SDL_SetRenderDrawColor(renderer, 217, 217, 217, 255);
				}
				else if (nodes[j * graph.graph_Height + i].visited) {
					SDL_SetRenderDrawColor(renderer, 153, 204, 255, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 102, 153, 255, 255);
				}
				if (&nodes[j * graph.graph_Height + i] == sn) {
					SDL_SetRenderDrawColor(renderer, 204, 255, 102, 255);
				}
				else if (&nodes[j * graph.graph_Height + i] == en) {
					SDL_SetRenderDrawColor(renderer, 255, 102, 102, 255);
				}
				SDL_RenderFillRect(renderer, &v_Node);
			}
		}

		//Drawing path from start to end
		if (en != nullptr) {
			Graph::Node* path = en;
			while (path->parent != nullptr) {
				SDL_SetRenderDrawColor(renderer, 252, 186, 3, 255);
				SDL_RenderDrawLine(renderer, path->col * node_Gap + node_Size / 2, path->row * node_Gap + node_Size / 2, path->parent->col * node_Gap + node_Size / 2, path->parent->row * node_Gap + node_Size / 2);
				SDL_Rect v_Node;
				v_Node.w = node_Size;
				v_Node.h = node_Size;
				v_Node.x = path->col * node_Gap;
				v_Node.y = path->row * node_Gap;
				if (path == en) {
					SDL_SetRenderDrawColor(renderer, 255, 102, 102, 255);
				}
				SDL_RenderFillRect(renderer, &v_Node);
				path = path->parent;
			}
		}
		if (algos != 1) {
			for (int i{ 0 }; i < graph.graph_Width; ++i) {
				for (int j{ 0 }; j < graph.graph_Height; ++j) {
					SDL_Rect v_Weight;
					v_Weight.w = node_Size - 20;
					v_Weight.h = node_Size - 20;
					v_Weight.x = i * node_Gap + 10;
					v_Weight.y = j * node_Gap + 10;
					if (nodes[j * graph.graph_Height + i].weight == 1) {
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					}
					else if (nodes[j * graph.graph_Height + i].weight == 2) {
						SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
					}
					else if (nodes[j * graph.graph_Height + i].weight == 3) {
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					}
					if (nodes[j * graph.graph_Height + i].weight != 0) {
						SDL_RenderFillRect(renderer, &v_Weight);
					}
				}
			}
		}
		SDL_RenderPresent(renderer);
	}
}
