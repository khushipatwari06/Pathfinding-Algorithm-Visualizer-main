#include "Algos.h"

Graph::Node::Node() {
	wall = false;
	visited = false;
	parent = nullptr;
	col = 0;
	row = 0;
	local = 1000;
	cost = 1000;
	weight = 0;
};

Graph::Graph() {
	nodes = new Node[16*16]; //Creating an array of nodes to hold values of each node
	for (int i{ 0 }; i < graph_Width; ++i) {
		for (int j{ 0 }; j < graph_Height; ++j) {
			nodes[i * graph_Height + j].col = j; //Giving each node positioning data
			nodes[i * graph_Height + j].row = i;
			nodes[i * graph_Height + j].wall = false;
			nodes[i * graph_Height + j].visited = false;
			nodes[i * graph_Height + j].parent = nullptr;
			nodes[i * graph_Height + j].local = 1000;
			nodes[i * graph_Height + j].cost = 1000;
			nodes[i * graph_Height + j].weight = 0;
		}
	}
	for (int i{ 0 }; i < graph_Width; ++i) {
		for (int j{ 0 }; j < graph_Height; ++j) {
			if (j > 0) {
				nodes[j * graph_Width + i].neighbours.push_back(&nodes[(j - 1) * graph_Width + i]);
			}
			if (j < graph_Height - 1) {
				nodes[j * graph_Width + i].neighbours.push_back(&nodes[(j + 1) * graph_Width + i]);
			}
			if (i > 0) {
				nodes[j * graph_Width + i].neighbours.push_back(&nodes[j * graph_Width + (i - 1)]);
			}
			if (i < graph_Width - 1) {
				nodes[j * graph_Width + i].neighbours.push_back(&nodes[j * graph_Width + (i + 1)]);
			}
		};
	};
	s_Node = &nodes[(graph_Height / 2) * graph_Width + 1]; //Giving starting node default value (1 node right of start of centre row)
	e_Node = &nodes[(graph_Height / 2) * graph_Width + graph_Width - 2]; //Giving end node default value (1 node left of end of centre row)
}

double Graph::manhattan(Node* a, Node* b) {
	if (a->weight == 0) {
		return (abs(b->col - a->col)+ abs(b->row - a->row));
	}
	else if (a->weight == 1) {
		return (abs(b->col - a->col)+ abs(b->row - a->row)) + 2;
	}
	else if (a->weight == 2) {
		return (abs(b->col - a->col)+ abs(b->row - a->row)) * 2;
	}
	else {
		return (abs(b->col - a->col)+ abs(b->row - a->row)) * 2 + 3;
	}
}

double Graph::dia_Distance(Node* a, Node* b) {
	if (a->weight == 0) {
		return std::max(abs(b->col - a->col), abs(b->row - a->row));
	}
	else if (a->weight == 1) {
		return (std::max(abs(b->col - a->col), abs(b->row - a->row))+3);
	}
	else if (a->weight == 2) {
		return (std::max(abs(b->col - a->col), abs(b->row - a->row))*2);
	}
	else {
		return (std::max(abs(b->col - a->col), abs(b->row - a->row)) * 2 + 3);
	}
}

double Graph::euclidean(Node* a, Node* b) {
	if (a->weight == 0) {
		return sqrt(pow((b->col - a->col), 2) + pow((b->row - a->row), 2));
	}
	else if (a->weight == 1) {
		return sqrt(pow((b->col - a->col), 2) + pow((b->row - a->row), 2)) + 3;
	}
	else if (a->weight == 2) {
		return (sqrt(pow((b->col - a->col), 2) + pow((b->row - a->row), 2))) * 2;
	}
	else {
		return (sqrt(pow((b->col - a->col), 2) + pow((b->row - a->row), 2))) * 2 + 3;
	}
}
double Graph::heuristic(Node* a, Node* b) {
	if (h == 0) {
		return manhattan(a, b);
	}
	if (h == 1) {
		return dia_Distance(a, b);
	}
	if (h == 2) {
		return euclidean(a, b);
	}
}


bool Graph::contained(Node* node, std::list<Node*> q) {
	for (auto n : q) {
		if (node == n) {
			return true;
		}
	}
	return false;
}

bool Graph::BFS() {
	//initiating graph
	for (int i{ 0 }; i < graph_Width; ++i) {
		for (int j{ 0 }; j < graph_Height; ++j) {
			nodes[j * graph_Height + i].visited = false;
			nodes[i * graph_Height + j].parent = nullptr;
			nodes[i * graph_Height + j].weight = 0;
		}
	}
	std::list<Node*> q; //initiating queue
	q.push_back(s_Node);
	Node* current = s_Node; 
	while (!q.empty()) {
		if (current == e_Node) {
			return true;
		}
		current = q.front();
		q.pop_front();
		current->visited = true;
		for (auto node_neighbour : current->neighbours){
			if (!node_neighbour->visited && !node_neighbour->wall && !contained(node_neighbour,q)) {
				node_neighbour->parent = current;
				q.push_back(node_neighbour);
			}
		}
	}
	return false;
}

bool Graph::Astar() {
	//Resetting graph for new traversal
	for (int i{ 0 }; i < graph_Width; ++i) {
		for (int j{ 0 }; j < graph_Height; ++j) {
			nodes[j * graph_Height + i].visited = false;
			nodes[i * graph_Height + j].parent = nullptr;
			nodes[i * graph_Height + j].local = INFINITY;
			nodes[i * graph_Height + j].cost = INFINITY;
		}
	}
	std::list<Node*> unvisited;
	unvisited.push_back(s_Node);
	s_Node->local = 0;
	s_Node->cost = heuristic(s_Node, e_Node);
	Node* current = s_Node;
	while (!unvisited.empty() ) {
		unvisited.sort([](const Node* a,const Node* b) {return a->cost < b->cost; }); //If a global is less than b global it is sorted to front of list
		current = unvisited.front();
		unvisited.pop_front();
		current->visited = true;
		if (current == e_Node) {
			return true;
		}
		
		for (auto node_Neighbour : current->neighbours) {
			if (!node_Neighbour->visited && !node_Neighbour->wall && current->cost < node_Neighbour->cost){
				node_Neighbour->parent = current;
				node_Neighbour->local = current->local + heuristic(current, node_Neighbour);
				node_Neighbour->cost = node_Neighbour->local + heuristic(node_Neighbour, e_Node);
				if (!contained(node_Neighbour, unvisited)) {
					unvisited.push_back(node_Neighbour);
				}
			}
		}
	}
	return false;
}

bool Graph::Djikstra() {
	//Resetting graph for new traversal
	for (int i{ 0 }; i < graph_Width; ++i) {
		for (int j{ 0 }; j < graph_Height; ++j) {
			nodes[j * graph_Height + i].visited = false;
			nodes[i * graph_Height + j].parent = nullptr;
			nodes[i * graph_Height + j].local = 1000;
		}
	}
	std::list<Node*> unvisited;
	unvisited.push_back(s_Node);
	s_Node->local = 0;
	Node* current = s_Node;
	while (!unvisited.empty()) {
		unvisited.sort([](const Node* a, const Node* b) {return a->local < b->local;});
		current = unvisited.front();
		unvisited.pop_front();
		current->visited = true;
		if (current == e_Node) {
			return true;
		}
		for (auto node_Neighbour : current->neighbours) {
			
			if (!node_Neighbour->visited && !node_Neighbour->wall && current->local + manhattan(current, node_Neighbour) < node_Neighbour->local) {
				node_Neighbour->parent = current;
				node_Neighbour->local = current->local + manhattan(current, node_Neighbour);
				if (!contained(node_Neighbour, unvisited)) {
					unvisited.push_back(node_Neighbour);
				}
			}
		}
	}
	return false;
}


void Graph::set_Start(int x, int y) {
	s_Node->visited = false;
	s_Node = &nodes[y * graph_Height + x];
	nodes[y * graph_Height + x].wall = false;
	nodes[y * graph_Height + x].visited = false;
}

void Graph::set_End(int x, int y) {
	e_Node = &nodes[y * graph_Height + x];
	nodes[y * graph_Height + x].wall = false;
	nodes[y * graph_Height + x].visited = false;
}

void Graph::switch_Wall(int x, int y) {
	if ((&nodes[y * graph_Height + x]) != s_Node && (&nodes[y * graph_Height + x]) != e_Node) {
		nodes[y * graph_Height + x].wall = !nodes[y * graph_Height + x].wall;
		nodes[y * graph_Height + x].weight = 0;
	}
	
}

void Graph::switch_Weight(int x, int y) {
	if ((&nodes[y * graph_Height + x]) != s_Node && (&nodes[y * graph_Height + x]) != e_Node) {
		nodes[y * graph_Height + x].weight += 1;
		if (nodes[y * graph_Height + x].weight == 4) {
			nodes[y * graph_Height + x].weight = 0;
		}
	}
}
